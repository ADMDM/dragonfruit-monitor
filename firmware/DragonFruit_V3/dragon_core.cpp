// DragonFruit V3 - ESP32 WROOM / Arduino-ESP32 3.x
// Anciennes donnees: /d<jour-epoch-UTC>.csv, jamais modifiees.
// Nouvelles donnees: /v3/d<jour-epoch-UTC>.csv, format separe.
// Pas de formatage LittleFS ni de changement de partition.
#include "dragon_core.h"
#include <Arduino.h>
#include "config.h"
#include "secrets.h"
#include "page.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>
#include <FS.h>
#include <LittleFS.h>
#include <Preferences.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include <time.h>
#include <math.h>
#include <esp_system.h>
#if defined(DRAGON_HEALTHCHECKS_ACTIVE) && DRAGON_HEALTHCHECKS_ACTIVE
#include <HTTPClient.h>
#endif

namespace {
WebServer web(80);
Preferences prefs;
BH1750 lightSensor;
Adafruit_AHTX0 ahtSensor;
Adafruit_BMP280 barometer;
OneWire oneWire(PIN_DS18B20);
DallasTemperature rootProbe(&oneWire);
WiFiClientSecure telegramTLS;
UniversalTelegramBot telegram(String(DRAGON_TG_TOKEN), telegramTLS);

struct Settings {
  float tLow = 15.0f;
  float tHigh = 29.0f;
  float rhLow = 35.0f;
  float rhHigh = 75.0f;
  float lightLow = 2000.0f; // Reperage provisoire, PAS un seuil biologique.
  bool telegram = true;
  bool lightAlerts = false; // desactive avant validation du BH1750 au feuillage
  int wetRaw = 0;  // reference APRES arrosage, jamais devinee
  int dryRaw = 0;  // reference AVANT arrosage, jamais devinee
  int positionRaw = 1279; // repere 5 min du 22/11/2026, PAS un % d'eau
};
Settings cfg;

struct Sample {
  uint32_t epoch = 0;
  uint32_t upMs = 0;
  int soil = -1;
  int soilMin = -1;
  int soilMax = -1;
  float temp = NAN;
  float rh = NAN;
  float pressure = NAN;
  float lux = NAN;
  float rootTemp = NAN;
};
Sample current;
static const size_t LIVE_CAPACITY = 180; // 15 min / echantillon 5 s
Sample liveSamples[LIVE_CAPACITY];
size_t liveHead = 0;
size_t liveCount = 0;

struct Episode {
  bool pending = false;
  bool active = false;
  uint32_t since = 0;
};
Episode coldEpisode, heatEpisode, dryAirEpisode, wetAirEpisode, rootLostEpisode;

struct Calibration {
  bool running = false;
  bool done = false;
  uint32_t started = 0;
  int values[16];
  int mins[16];
  int maxs[16];
  uint8_t count = 0;
  int median = -1;
  int lo = -1;
  int hi = -1;
  uint32_t completedEpoch = 0;
};
Calibration cal;

bool filesystemOK = false;
bool ahtOK = false;
bool bmpOK = false;
bool bhOK = false;
bool dsOK = false;
bool hadDS = false;
uint32_t lastSampleMs = 0;
uint32_t lastDiskMs = 0;
uint32_t lastSensorRetryMs = 0;
uint32_t lastWifiRetryMs = 0;
uint32_t lastBotPollMs = 0;
uint32_t lastSavedEpoch = 0;
uint32_t lastWaterEpoch = 0;
uint32_t positionStartEpoch = 0;
float soilDailySlope = NAN;
float daysUntilCheck = NAN;
bool lastSaveOK = true;
bool storageFault = false;
bool storageFaultSent = false;
bool storageRecoveredPending = false;
uint32_t monitorNotifyAttemptMs = 0;
uint32_t wifiDownAtMs = 0;
uint32_t recoveredWifiMinutes = 0;
bool wifiRecoveredPending = false;
int bootResetReason = 0;
int lastHeartbeatHttp = 0;
bool mdnsOK = false;
bool wifiAnnounced = false;
uint32_t lastMdnsAttemptMs=0;
uint32_t dsRequestedAt = 0;
bool dsWaiting = false;

bool hasTime() {
  time_t t = time(nullptr);
  return t > 1700000000;
}
uint32_t nowEpoch() { return hasTime() ? (uint32_t)time(nullptr) : 0U; }
bool valid(float value) { return isfinite(value); }

String num(float value, int digits=2) {
  return valid(value) ? String(value,digits) : String("null");
}
String jsonBool(bool value) { return value ? "true" : "false"; }
String jsonQuote(const String &s) {
  String out = "\"";
  for(size_t i=0;i<s.length();i++) {
    char c=s[i];
    if(c=='"' || c=='\\') { out+='\\'; out+=c; }
    else if(c=='\n') out+="\\n";
    else if((uint8_t)c>=32) out+=c;
  }
  return out+'"';
}
void jsonReply(int status,const String& content) {
  web.sendHeader("Cache-Control","no-store");
  web.send(status,"application/json; charset=utf-8",content);
}
void errorReply(int status,const String &message) {
  jsonReply(status,"{\"ok\":false,\"message\":"+jsonQuote(message)+"}");
}
bool canWrite() {
  String expected = String(DRAGON_LOCAL_CODE);
  if(expected.length()<8) {
    errorReply(503,"Definis d'abord un code local d'au moins 8 caracteres dans secrets.h");
    return false;
  }
  if(web.header("X-Dragon-Key")!=expected) {
    errorReply(403,"Code de controle incorrect");
    return false;
  }
  return true;
}

void loadSettings() {
  prefs.begin("dragonv3", false);
  cfg.tLow=prefs.getFloat("tLow",15.0f);
  cfg.tHigh=prefs.getFloat("tHigh",29.0f);
  cfg.rhLow=prefs.getFloat("rhLow",35.0f);
  cfg.rhHigh=prefs.getFloat("rhHigh",75.0f);
  cfg.lightLow=prefs.getFloat("luxLow",2000.0f);
  cfg.telegram=prefs.getBool("tgOn",true);
  cfg.lightAlerts=prefs.getBool("lightOn",false);
  cfg.wetRaw=prefs.getInt("wetRaw",0);
  cfg.dryRaw=prefs.getInt("dryRaw",0);
  cfg.positionRaw=prefs.getInt("position",1279);
  hadDS=prefs.getBool("seenDS",false);
  lastWaterEpoch=prefs.getUInt("waterEpoch",0);
  positionStartEpoch=prefs.getUInt("posEpoch",0);
}
void saveSettings() {
  prefs.putFloat("tLow",cfg.tLow);
  prefs.putFloat("tHigh",cfg.tHigh);
  prefs.putFloat("rhLow",cfg.rhLow);
  prefs.putFloat("rhHigh",cfg.rhHigh);
  prefs.putFloat("luxLow",cfg.lightLow);
  prefs.putBool("tgOn",cfg.telegram);
  prefs.putBool("lightOn",cfg.lightAlerts);
  prefs.putInt("wetRaw",cfg.wetRaw);
  prefs.putInt("dryRaw",cfg.dryRaw);
  prefs.putInt("position",cfg.positionRaw);
}

void initSensors() {
  Wire.begin(PIN_SDA,PIN_SCL);
  ahtOK=ahtSensor.begin(&Wire);
  bmpOK=barometer.begin(0x76) || barometer.begin(0x77);
  bhOK=lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE,0x23,&Wire);
  rootProbe.begin();
  rootProbe.setResolution(12);
  rootProbe.setWaitForConversion(false);
  dsOK=rootProbe.getDeviceCount()>0;
  Serial.printf("I2C: AHT=%d BMP=%d BH1750=%d / DS18B20=%d\n",ahtOK,bmpOK,bhOK,dsOK);
}
void retrySensors() {
  if(millis()-lastSensorRetryMs<60000) return;
  lastSensorRetryMs=millis();
  if(!ahtOK) ahtOK=ahtSensor.begin(&Wire);
  if(!bmpOK) bmpOK=barometer.begin(0x76) || barometer.begin(0x77);
  if(!bhOK) bhOK=lightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE,0x23,&Wire);
  if(!dsOK) { rootProbe.begin(); dsOK=rootProbe.getDeviceCount()>0; }
}
int soilMedian(int &mi,int &ma) {
  const int N=21;
  int readings[N];
  mi=4095; ma=0;
  for(int i=0;i<N;i++) {
    int v=analogRead(PIN_SOIL);
    readings[i]=v;
    if(v<mi) mi=v;
    if(v>ma) ma=v;
    delay(2);
  }
  for(int i=1;i<N;i++) {
    int v=readings[i],j=i-1;
    while(j>=0 && readings[j]>v) { readings[j+1]=readings[j]; j--; }
    readings[j+1]=v;
  }
  return readings[N/2];
}
void readDS18B20() {
  if(!dsWaiting) {
    if(!dsOK || (millis()-dsRequestedAt<SAMPLE_MS)) return;
    rootProbe.requestTemperatures();
    dsRequestedAt=millis();
    dsWaiting=true;
    return;
  }
  if(millis()-dsRequestedAt<800) return;
  dsWaiting=false;
  float t=rootProbe.getTempCByIndex(0);
  if(t> -55 && t<85 && fabs(t-85.0f)>0.01f && t != DEVICE_DISCONNECTED_C) {
    current.rootTemp=t;
    dsOK=true;
    if(!hadDS) {hadDS=true; prefs.putBool("seenDS",true);}
  } else {
    current.rootTemp=NAN;
    dsOK=false;
  }
}
void pushLive() {
  liveSamples[liveHead]=current;
  liveHead=(liveHead+1)%LIVE_CAPACITY;
  if(liveCount<LIVE_CAPACITY) liveCount++;
}
void readSensors() {
  current.soil=soilMedian(current.soilMin,current.soilMax);
  current.temp=NAN; current.rh=NAN; current.pressure=NAN; current.lux=NAN;
  if(ahtOK) {
    sensors_event_t hum,temp;
    if(ahtSensor.getEvent(&hum,&temp)) {
      current.temp=temp.temperature;
      current.rh=hum.relative_humidity;
    } else ahtOK=false;
  }
  if(bmpOK) {
    float p=barometer.readPressure()/100.0f;
    if(valid(p) && p>500 && p<1200) current.pressure=p;
    else bmpOK=false;
  }
  if(bhOK) {
    float x=lightSensor.readLightLevel();
    if(valid(x) && x>=0 && x<=100000) current.lux=x;
    else bhOK=false;
  }
  current.epoch=nowEpoch();
  current.upMs=millis();
  pushLive();
}

// Estimation uniquement apres >12h d'observations dans la NOUVELLE position
// et apres le dernier arrosage. Lecture dans les fichiers V3, jamais l'ancien CSV.
void recalcSoilTrend() {
  soilDailySlope=NAN;
  daysUntilCheck=NAN;
  uint32_t now=nowEpoch();
  if(!filesystemOK || now==0)return;
  uint32_t start=now-36U*3600U;
  if(lastWaterEpoch>start)start=lastWaterEpoch;
  if(positionStartEpoch>start)start=positionStartEpoch;
  uint32_t firstT=0,lastT=0;
  int firstR=0,lastR=0;
  int count=0;
  for(uint32_t day=start/86400UL;day<=now/86400UL;day++){
    String path="/v3/d"+String(day)+".csv";
    if(!LittleFS.exists(path))continue;
    File f=LittleFS.open(path,"r");
    if(!f)continue;
    while(f.available()) {
      String line=f.readStringUntil('\n');
      int semi=line.indexOf(';');
      if(semi<9)continue;
      uint32_t epoch=(uint32_t)strtoul(line.substring(0,semi).c_str(),nullptr,10);
      if(epoch<start || epoch>now)continue;
      int nextSemi=line.indexOf(';',semi+1);
      if(nextSemi<0)continue;
      int raw=line.substring(semi+1,nextSemi).toInt();
      if(raw<100 || raw>4090)continue;
      if(firstT==0){firstT=epoch;firstR=raw;}
      lastT=epoch;lastR=raw;count++;
    }
    f.close();
  }
  if(count<12 || lastT-firstT<12UL*3600UL)return;
  float rate=((float)(lastR-firstR))*86400.0f/(lastT-firstT);
  if(rate<3.0f || rate>350.0f)return; // incoherence ou trop bruité
  soilDailySlope=rate;
  if(cfg.wetRaw>0 && cfg.dryRaw>cfg.wetRaw+30 && current.soil>=0) {
    float remaining=((float)cfg.dryRaw-current.soil)/rate;
    if(remaining>=0 && remaining<21)daysUntilCheck=remaining;
  }
}

bool appendLine(const String &path,const String &header,const String &line) {
  if(!filesystemOK) return false;
  bool exists=LittleFS.exists(path);
  File f=LittleFS.open(path,"a");
  if(!f) { lastSaveOK=false; return false; }
  bool ok=true;
  if(!exists) ok=f.println(header)>0;
  if(ok) ok=f.println(line)>0;
  f.flush(); f.close();
  if(!ok) lastSaveOK=false;
  return ok;
}
String csvFloat(float v){ return valid(v)?String(v,2):String(""); }
void pingExternalMonitor();
void saveSample() {
  uint32_t epoch=nowEpoch();
  // L'horloge peut etre indisponible au demarrage. Ne jamais ecrire sans date.
  if(!filesystemOK || (!epoch && millis()>15UL*60000UL)) {
    lastSaveOK=false; storageFault=true;
    Serial.println("DragonFruit : stockage ou horloge indisponible");
    return;
  }
  if(!epoch) return;
  // Jamais ecrire dans /d<jour>.csv de V2.2 (anciens en-tetes differents).
  String path="/v3/d"+String(epoch/86400UL)+".csv";
  String row=String(epoch)+";"+String(current.soil)+";"+
    String(current.soilMin)+";"+String(current.soilMax)+";"+
    csvFloat(current.temp)+";"+csvFloat(current.rh)+";"+
    csvFloat(current.pressure)+";"+csvFloat(current.lux)+";"+csvFloat(current.rootTemp);
  if(appendLine(path,"epoch;raw;min;max;temperature;humidite;pression;lux;sol_temp",row)) {
    lastSavedEpoch=epoch; lastSaveOK=true;
    if(storageFault && storageFaultSent)storageRecoveredPending=true;
    storageFault=false;
    storageFaultSent=false;
    recalcSoilTrend();
    pingExternalMonitor();
  } else {
    storageFault=true;lastSaveOK=false;
    Serial.println("DragonFruit : echec de la sauvegarde CSV");
  }
}

bool telegramReady() {
  return cfg.telegram && String(DRAGON_TG_TOKEN).length()>20 &&
    String(DRAGON_TG_CHAT_ID).length()>0 &&
    WiFi.status()==WL_CONNECTED && hasTime();
}
bool sendTelegram(const String &text) {
  if(!telegramReady()) return false;
  bool result=telegram.sendMessage(String(DRAGON_TG_CHAT_ID),text,"");
  Serial.printf("Telegram send: %d\n",(int)result);
  return result;
}
// Detecter erreurs de stockage, retour Wi-Fi et avertir quand Internet fonctionne.
void serviceDiagnostics() {
  if(millis()-monitorNotifyAttemptMs<60000UL) return;
  if(!telegramReady()) return;
  if(storageFault && !storageFaultSent) {
    monitorNotifyAttemptMs=millis();
    if(sendTelegram("DragonFruit : echec de sauvegarde sur LittleFS ou horloge indisponible. Verifie /api/status et le stockage."))storageFaultSent=true;
    return;
  }
  if(storageRecoveredPending) {
    monitorNotifyAttemptMs=millis();
    if(sendTelegram("DragonFruit : sauvegarde retablie. Les nouvelles mesures sont de nouveau ecrites."))storageRecoveredPending=false;
    return;
  }
  if(wifiRecoveredPending) {
    monitorNotifyAttemptMs=millis();
    if(sendTelegram("DragonFruit : connexion Wi-Fi retablie apres environ "+String(recoveredWifiMinutes)+" minute(s) hors ligne."))wifiRecoveredPending=false;
  }
}

// Uniquement si un Healthchecks personnel est configure localement.
// Le moniteur externe signale les coupures totales et les absences de sauvegardes.
// Le ping n'est emis qu'apres une sauvegarde reussie (toutes les 5 minutes).
void pingExternalMonitor() {
#if defined(DRAGON_HEALTHCHECKS_ACTIVE) && DRAGON_HEALTHCHECKS_ACTIVE
  String url=String(DRAGON_HEALTHCHECKS_URL);
  if(!url.startsWith("https://hc-ping.com/") || WiFi.status()!=WL_CONNECTED || !hasTime() || !lastSaveOK || storageFault)return;
  WiFiClientSecure client;
  // Compromis : pas de bundle CA additionnel dans le flash quasi plein.
  // La verification TLS doit etre ajoutee lors d'une optimisation du firmware.
  client.setInsecure();
  HTTPClient http;
  http.setConnectTimeout(3000);
  http.setTimeout(3000);
  if(!http.begin(client,url))return;
  lastHeartbeatHttp=http.GET();
  Serial.printf("DragonFruit heartbeat HTTP : %d\n",lastHeartbeatHttp);
  http.end();
#endif
}
void episode(Episode &e,bool condition,uint32_t confirmMs,const String &message) {
  if(!condition) {e.pending=false; e.active=false; return;}
  if(e.active) return;
  if(!e.pending) { e.pending=true; e.since=millis(); }
  if((uint32_t)(millis()-e.since)>=confirmMs) {
    e.active=true;
    // Une notification maximum pour l'episode confirme.
    sendTelegram("DragonFruit - " + message);
  }
}
void checkAlerts() {
  episode(coldEpisode,valid(current.temp)&&current.temp<cfg.tLow,30UL*60000UL,
    "temperature basse : "+num(current.temp,1)+" C. Verifie l'emplacement.");
  episode(heatEpisode,valid(current.temp)&&current.temp>cfg.tHigh,30UL*60000UL,
    "temperature elevee : "+num(current.temp,1)+" C. Verifie la chaleur.");
  episode(dryAirEpisode,valid(current.rh)&&current.rh<cfg.rhLow,120UL*60000UL,
    "air sec : "+num(current.rh,0)+" % HR. Verifie les conditions.");
  episode(wetAirEpisode,valid(current.rh)&&current.rh>cfg.rhHigh,360UL*60000UL,
    "air tres humide : "+num(current.rh,0)+" % HR. Verifie la ventilation.");
  episode(rootLostEpisode,hadDS&&!dsOK,5UL*60000UL,
    "DS18B20 absent depuis 5 minutes. Verifie le cable GPIO27.");
  // Ne pas declencher d'arrosage automatique sans calibrations valides.
  // Lumiere: pas d'alerte a partir d'une mesure ponctuelle. Analyse multi-jours dans la page.
}

int medianN(const int *values,int n) {
  int v[16];
  if(n<1 || n>16) return -1;
  for(int i=0;i<n;i++) v[i]=values[i];
  for(int i=1;i<n;i++) {int x=v[i],j=i-1; while(j>=0 && v[j]>x){v[j+1]=v[j]; j--;}v[j+1]=x;}
  return n%2?v[n/2]:(v[n/2-1]+v[n/2])/2;
}
void calibrationTick() {
  if(!cal.running || cal.count>=16) return;
  if((uint32_t)(millis()-cal.started) < (uint32_t)cal.count*CALIBRATION_STEP_MS) return;
  cal.values[cal.count]=current.soil;
  cal.mins[cal.count]=current.soilMin;
  cal.maxs[cal.count]=current.soilMax;
  cal.count++;
  if(cal.count==16) {
    cal.running=false;
    cal.done=true;
    cal.median=medianN(cal.values,16);
    cal.lo=4095;cal.hi=0;
    for(int i=0;i<16;i++) {if(cal.values[i]<cal.lo)cal.lo=cal.values[i];if(cal.values[i]>cal.hi)cal.hi=cal.values[i];}
    cal.completedEpoch=nowEpoch();
    cfg.positionRaw=cal.median;
    prefs.putInt("position",cfg.positionRaw);
    appendLine("/v3/calibrations.csv","epoch;median;min;max;amplitude;type",
      String(cal.completedEpoch)+";"+cal.median+";"+cal.lo+";"+cal.hi+";"+(cal.hi-cal.lo)+";stabilite_position");
  }
}

String recommendations() {
  if(cfg.wetRaw<=0 || cfg.dryRaw<=0 || cfg.dryRaw<=cfg.wetRaw+30)
    return "Substrat : arrosage non previsible avant deux references fiables. Verifie le terreau au toucher.";
  float relative = 100.0f*(cfg.dryRaw-current.soil)/(cfg.dryRaw-cfg.wetRaw);
  if(relative<=15.0f) return "Substrat proche du repere avant arrosage : verifier manuellement aujourd'hui.";
  if(relative<35.0f) return "Substrat en cours de sechage : surveiller avant d'arroser.";
  return "Substrat dans la plage observee apres arrosage. Ne pas arroser sur ce seul indice.";
}
String buildStatus() {
  String s; s.reserve(1400);
  s="{\"name\":\"DragonFruit - Faux Poivrier\",\"version\":\"3.0\"";
  s+=",\"epoch\":"+String(nowEpoch());
  s+=",\"lastMeasurement\":"+String(current.epoch);
  s+=",\"lastSaved\":"+String(lastSavedEpoch);
  s+=",\"uptimeSeconds\":"+String(millis()/1000UL);
  s+=",\"wifi\":"+jsonBool(WiFi.status()==WL_CONNECTED);
  s+=",\"ip\":"+jsonQuote(WiFi.localIP().toString());
  s+=",\"rssi\":"+String(WiFi.status()==WL_CONNECTED?WiFi.RSSI():0);
  s+=",\"clock\":"+jsonBool(hasTime());
  s+=",\"fs\":"+jsonBool(filesystemOK);
  s+=",\"fsUsed\":"+String(filesystemOK?LittleFS.usedBytes():0);
  s+=",\"fsTotal\":"+String(filesystemOK?LittleFS.totalBytes():0);
  s+=",\"lastSaveOK\":"+jsonBool(lastSaveOK);
  s+=",\"storageFault\":"+jsonBool(storageFault);
  s+=",\"resetReason\":"+String(bootResetReason);
  s+=",\"freeHeap\":"+String(ESP.getFreeHeap());
  s+=",\"lastHeartbeatHttp\":"+String(lastHeartbeatHttp);
  s+=",\"soilSlope\":"+num(soilDailySlope,1);
  s+=",\"daysToCheck\":"+num(daysUntilCheck,1);
  s+=",\"soil\":"+String(current.soil);
  s+=",\"soilMin\":"+String(current.soilMin);
  s+=",\"soilMax\":"+String(current.soilMax);
  s+=",\"temperature\":"+num(current.temp);
  s+=",\"humidity\":"+num(current.rh);
  s+=",\"pressure\":"+num(current.pressure);
  s+=",\"lux\":"+num(current.lux);
  s+=",\"soilTemperature\":"+num(current.rootTemp);
  s+=",\"ahtOK\":"+jsonBool(ahtOK);
  s+=",\"bmpOK\":"+jsonBool(bmpOK);
  s+=",\"bhOK\":"+jsonBool(bhOK);
  s+=",\"dsOK\":"+jsonBool(dsOK);
  s+=",\"dsEver\":"+jsonBool(hadDS);
  s+=",\"telegramConfigured\":"+jsonBool(String(DRAGON_TG_TOKEN).length()>20);
  s+=",\"config\":{\"tLow\":"+num(cfg.tLow,1)+",\"tHigh\":"+num(cfg.tHigh,1);
  s+=",\"rhLow\":"+num(cfg.rhLow,1)+",\"rhHigh\":"+num(cfg.rhHigh,1);
  s+=",\"lightLow\":"+num(cfg.lightLow,0);
  s+=",\"telegram\":"+jsonBool(cfg.telegram);
  s+=",\"lightAlerts\":"+jsonBool(cfg.lightAlerts);
  s+=",\"positionRaw\":"+String(cfg.positionRaw);
  s+=",\"wetRaw\":"+String(cfg.wetRaw);
  s+=",\"dryRaw\":"+String(cfg.dryRaw)+"}";
  s+=",\"calibration\":{\"running\":"+jsonBool(cal.running)+",\"done\":"+jsonBool(cal.done);
  s+=",\"count\":"+String(cal.count)+",\"median\":"+String(cal.median);
  s+=",\"min\":"+String(cal.lo)+",\"max\":"+String(cal.hi);
  s+=",\"completedEpoch\":"+String(cal.completedEpoch)+"}";
  s+=",\"alerts\":{\"cold\":"+jsonBool(coldEpisode.active);
  s+=",\"heat\":"+jsonBool(heatEpisode.active);
  s+=",\"dryAir\":"+jsonBool(dryAirEpisode.active);
  s+=",\"wetAir\":"+jsonBool(wetAirEpisode.active);
  s+=",\"dsLost\":"+jsonBool(rootLostEpisode.active)+"}";
  s+=",\"advice\":"+jsonQuote(recommendations());
  s+="}";
  return s;
}

void apiStatus(){ jsonReply(200,buildStatus()); }
void apiLive(){
  web.setContentLength(CONTENT_LENGTH_UNKNOWN);
  web.send(200,"application/json; charset=utf-8","");
  web.sendContent("[");
  for(size_t i=0;i<liveCount;i++) {
    const Sample &p=liveSamples[(liveHead+LIVE_CAPACITY-liveCount+i)%LIVE_CAPACITY];
    String s=(i?",":"");s+="{\"epoch\":"+String(p.epoch);
    s+=",\"uptime\":"+String(p.upMs/1000);
    s+=",\"soil\":"+String(p.soil);
    s+=",\"temperature\":"+num(p.temp);
    s+=",\"humidity\":"+num(p.rh);
    s+=",\"pressure\":"+num(p.pressure);
    s+=",\"lux\":"+num(p.lux);
    s+=",\"soilTemperature\":"+num(p.rootTemp)+"}";
    web.sendContent(s);
  }
  web.sendContent("]"); web.sendContent("");
}

// CSV parser for both historic V2.2 and new V3 records; leaves blank missing cells.
int splitCSV(const String &line,String cols[],int capacity) {
  int n=0,begin=0;
  for(size_t i=0;i<=line.length();i++) {
    if(i==line.length() || line[i]==';') {
      if(n<capacity) {cols[n]=line.substring(begin,i);cols[n].trim();n++;}
      begin=i+1;
    }
  }
  return n;
}
String normalizedCSV(const String &line,const char *origin) {
  String cols[9];
  int n=splitCSV(line,cols,9);
  if(n<4 || cols[0].length()<9) return "";
  uint32_t stamp=(uint32_t)strtoul(cols[0].c_str(),nullptr,10);
  if(stamp<1700000000UL) return "";
  String row;
  for(int i=0;i<9;i++) {
    if(i)row+=';';
    row+=i<n?cols[i]:"";
  }
  row+=';';row+=origin;
  return row;
}
void streamCSVFile(const String &path,uint32_t since,bool jsonMode,bool &first,uint32_t stride,uint32_t &lastSent,const char* origin) {
  if(!LittleFS.exists(path)) return;
  File f=LittleFS.open(path,"r");
  if(!f)return;
  while(f.available()) {
    String line=f.readStringUntil('\n');line.trim();
    String cols[9];int n=splitCSV(line,cols,9);
    if(n<4 || cols[0].length()<9) continue;
    uint32_t stamp=(uint32_t)strtoul(cols[0].c_str(),nullptr,10);
    if(stamp<since || stamp<1700000000UL) continue;
    if(stride>0 && lastSent && stamp-lastSent<stride)continue;
    if(jsonMode) {
      String s=first?"":",";first=false;
      s+="{\"epoch\":"+String(stamp)+",\"soil\":"+cols[1];
      s+=",\"temperature\":"+(n>4 && cols[4].length()?cols[4]:"null");
      s+=",\"humidity\":"+(n>5 && cols[5].length()?cols[5]:"null");
      s+=",\"pressure\":"+(n>6 && cols[6].length()?cols[6]:"null");
      s+=",\"lux\":"+(n>7 && cols[7].length()?cols[7]:"null");
      s+=",\"soilTemperature\":"+(n>8 && cols[8].length()?cols[8]:"null")+"}";
      web.sendContent(s);
    } else {
      String row=normalizedCSV(line,origin);
      if(row.length())web.sendContent(row+"\n");
    }
    lastSent=stamp;
    yield();
  }
  f.close();
}
void streamHistory(bool jsonMode) {
  if(!filesystemOK) {errorReply(503,"Stockage LittleFS indisponible");return;}
  uint32_t until=nowEpoch();
  if(until==0) {errorReply(503,"Horloge NTP non synchronisee");return;}
  int h=web.hasArg("hours")?web.arg("hours").toInt():168;
  if(h!=1 && h!=6 && h!=24 && h!=168 && h!=720) h=168;
  uint32_t since=until-(uint32_t)h*3600UL;
  uint32_t stride=h==720?3600UL:(h==168?900UL:0UL);
  if(!jsonMode) {
    web.sendHeader("Content-Disposition","attachment; filename=dragonfruit_historique.csv");
  }
  web.setContentLength(CONTENT_LENGTH_UNKNOWN);
  web.send(200,jsonMode?"application/json; charset=utf-8":"text/csv; charset=utf-8","");
  if(jsonMode)web.sendContent("[");
  else web.sendContent("epoch;raw;min;max;temperature;humidite;pression;lux;sol_temp;origine\n");
  bool first=true;
  for(uint32_t day=since/86400UL;day<=until/86400UL;day++) {
    uint32_t lastOld=0,lastV3=0;
    streamCSVFile("/d"+String(day)+".csv",since,jsonMode,first,stride,lastOld,"V2.2");
    streamCSVFile("/v3/d"+String(day)+".csv",since,jsonMode,first,stride,lastV3,"V3");
  }
  if(jsonMode)web.sendContent("]");
  web.sendContent("");
}

void serveArrosages() {
  if(!filesystemOK){errorReply(503,"LittleFS indisponible");return;}
  if(!LittleFS.exists("/arrosages.csv")){web.send(200,"text/csv; charset=utf-8","epoch\n");return;}
  File f=LittleFS.open("/arrosages.csv","r");
  if(!f){errorReply(500,"Journal inaccessible");return;}
  web.sendHeader("Content-Disposition","inline; filename=dragonfruit_arrosages.csv");
  web.streamFile(f,"text/csv; charset=utf-8");f.close();
}
void apiWater() {
  if(!canWrite())return;
  if(!hasTime()) {errorReply(503,"Horloge non synchronisee, arrosage non enregistre");return;}
  if(!appendLine("/arrosages.csv","epoch",String(nowEpoch()))) {errorReply(507,"Memoire pleine ou non accessible");return;}
  lastWaterEpoch=nowEpoch(); prefs.putUInt("waterEpoch",lastWaterEpoch);
  soilDailySlope=NAN;daysUntilCheck=NAN;
  jsonReply(200,"{\"ok\":true,\"message\":\"Arrosage enregistre\"}");
}
void apiCalStart() {
  if(!canWrite())return;
  if(cal.running){errorReply(409,"Calibration deja en cours");return;}
  cal.running=true;cal.done=false;cal.count=0;cal.median=-1;cal.lo=-1;cal.hi=-1;cal.started=millis();
  jsonReply(200,"{\"ok\":true,\"message\":\"Releve de 5 minutes lance, sonde immobile\"}");
}
void apiCalMoved() {
  if(!canWrite())return;
  cfg.wetRaw=0;cfg.dryRaw=0;
  positionStartEpoch=nowEpoch();prefs.putUInt("posEpoch",positionStartEpoch);
  soilDailySlope=NAN;daysUntilCheck=NAN;
  saveSettings();
  appendLine("/v3/events.csv","epoch;event;soil",String(nowEpoch())+";sonde_deplacee;"+String(current.soil));
  jsonReply(200,"{\"ok\":true,\"message\":\"Anciennes references d'arrosage invalidees\"}");
}
void apiCalRef() {
  if(!canWrite())return;
  String kind=web.arg("kind");
  if(current.soil<1 || current.soil>4094){errorReply(422,"Sonde sol indisponible");return;}
  if(kind=="before")cfg.dryRaw=current.soil;
  else if(kind=="after")cfg.wetRaw=current.soil;
  else {errorReply(400,"Type inconnu");return;}
  saveSettings();
  recalcSoilTrend();
  appendLine("/v3/calibrations.csv","epoch;median;min;max;amplitude;type",
    String(nowEpoch())+";"+String(current.soil)+";"+String(current.soilMin)+";"+
    String(current.soilMax)+";"+String(current.soilMax-current.soilMin)+";"+kind);
  jsonReply(200,"{\"ok\":true,\"message\":\"Reference enregistree; verifier sur plusieurs cycles\"}");
}
void apiSettings() {
  if(!canWrite())return;
  float tl=web.arg("tLow").toFloat(),th=web.arg("tHigh").toFloat();
  float hl=web.arg("rhLow").toFloat(),hh=web.arg("rhHigh").toFloat();
  float lux=web.arg("lightLow").toFloat();
  if(tl<0 || tl>26 || th<tl+3 || th>45 || hl<10 || hl>75 || hh<hl+5 || hh>99 || lux<100 || lux>50000) {
    errorReply(422,"Seuils incoherents, aucun changement enregistre");return;
  }
  cfg.tLow=tl;cfg.tHigh=th;cfg.rhLow=hl;cfg.rhHigh=hh;cfg.lightLow=lux;
  cfg.telegram=web.arg("telegram")=="1";
  cfg.lightAlerts=web.arg("lightAlerts")=="1";
  saveSettings();
  jsonReply(200,"{\"ok\":true,\"message\":\"Reglages sauvegardes\"}");
}
void apiBotTest() {
  if(!canWrite())return;
  if(!telegramReady()){errorReply(503,"Telegram indisponible: token, Wi-Fi ou heure");return;}
  bool ok=sendTelegram("DragonFruit V3 : test depuis ton ESP32 reussi.");
  jsonReply(ok?200:502,ok?"{\"ok\":true,\"message\":\"Message Telegram envoye\"}":"{\"ok\":false,\"message\":\"Echec d'envoi Telegram\"}");
}
String telegramSummary() {
  String t="DragonFruit - Faux Poivrier\n";
  t+="Substrat : "+String(current.soil)+" (valeur brute)\n";
  t+="Air : "+num(current.temp,1)+" C / "+num(current.rh,0)+" % HR\n";
  t+="Lumiere : "+num(current.lux,0)+" lux\n";
  t+="Pression : "+num(current.pressure,0)+" hPa\n";
  t+="Racines : "+(dsOK?num(current.rootTemp,1)+" C":String("capteur en attente"))+"\n";
  t+=recommendations();
  return t;
}
void telegramPoll() {
  if(!telegramReady())return;
  if(millis()-lastBotPollMs<45000UL) return;
  lastBotPollMs=millis();
  int count=telegram.getUpdates(telegram.last_message_received+1);
  if(count<=0)return;
  for(int i=0;i<count;i++){
    if(telegram.messages[i].chat_id!=String(DRAGON_TG_CHAT_ID))continue;
    String cmd=telegram.messages[i].text;cmd.trim();
    if(cmd.startsWith("/status") || cmd.startsWith("/etat"))sendTelegram(telegramSummary());
    else if(cmd.startsWith("/help") || cmd.startsWith("/start"))
      sendTelegram("DragonFruit V3 : /status pour les mesures, /help pour l'aide. Les changements se font sur la page locale.");
  }
}

void serviceWifi() {
  if(WiFi.status()==WL_CONNECTED) {
    if(wifiDownAtMs!=0) {
      uint32_t downtime=millis()-wifiDownAtMs;
      if(downtime>=60000UL) {recoveredWifiMinutes=downtime/60000UL;wifiRecoveredPending=true;}
      wifiDownAtMs=0;
    }
    if(!wifiAnnounced) {
      wifiAnnounced=true;
      Serial.print("DragonFruit IP : ");Serial.println(WiFi.localIP().toString());
    }
    if(!mdnsOK && (lastMdnsAttemptMs==0 || millis()-lastMdnsAttemptMs>15000UL)) {
      lastMdnsAttemptMs=millis();
      mdnsOK=MDNS.begin("dragonfruit");
      if(mdnsOK) {MDNS.addService("http","tcp",80);Serial.println("Nom local : http://dragonfruit.local");}
    }
    return;
  }
  wifiAnnounced=false;
  if(wifiDownAtMs==0)wifiDownAtMs=millis();
  if(mdnsOK){MDNS.end();mdnsOK=false;}
  if(millis()-lastWifiRetryMs<30000UL)return;
  lastWifiRetryMs=millis();
  if(String(DRAGON_WIFI_SSID).length()>0) {
    WiFi.disconnect(false);
    WiFi.begin(DRAGON_WIFI_SSID,DRAGON_WIFI_PASSWORD);
    Serial.println("Tentative de reconnexion Wi-Fi...");
  }
}
void registerRoutes() {
  const char *names[]={"X-Dragon-Key"};
  web.collectHeaders(names,1);
  web.on("/",HTTP_GET,[](){web.send_P(200,"text/html; charset=utf-8",DRAGON_PAGE);});
  web.on("/api/status",HTTP_GET,apiStatus);
  web.on("/api/live",HTTP_GET,apiLive);
  web.on("/api/history",HTTP_GET,[](){streamHistory(true);});
  web.on("/export.csv",HTTP_GET,[](){streamHistory(false);});
  web.on("/arrosages.csv",HTTP_GET,serveArrosages);
  web.on("/api/water",HTTP_POST,apiWater);
  web.on("/api/calibration/start",HTTP_POST,apiCalStart);
  web.on("/api/calibration/moved",HTTP_POST,apiCalMoved);
  web.on("/api/calibration/reference",HTTP_POST,apiCalRef);
  web.on("/api/settings",HTTP_POST,apiSettings);
  web.on("/api/telegram/test",HTTP_POST,apiBotTest);
  web.onNotFound([](){web.send(404,"text/plain; charset=utf-8","Page introuvable");});
  web.begin();
}
} // namespace

void dragonSetup() {
  Serial.begin(115200);
  delay(150);
  Serial.println("\n=== DragonFruit V3.1 ===");
  bootResetReason=(int)esp_reset_reason();
  Serial.printf("Cause du redemarrage (esp_reset_reason) : %d\n",bootResetReason);
  analogReadResolution(12);
  analogSetPinAttenuation(PIN_SOIL,ADC_11db);
  loadSettings();
  filesystemOK=LittleFS.begin(false); // SURTOUT pas de formatage automatique
  if(filesystemOK)LittleFS.mkdir("/v3");
  Serial.printf("LittleFS monte: %d\n",filesystemOK);
  initSensors();
  WiFi.mode(WIFI_STA);
  WiFi.setSleep(false);
  WiFi.setAutoReconnect(true);
  if(String(DRAGON_WIFI_SSID).length()>0)WiFi.begin(DRAGON_WIFI_SSID,DRAGON_WIFI_PASSWORD);
  configTzTime("CET-1CEST,M3.5.0,M10.5.0/3","pool.ntp.org","time.google.com","time.cloudflare.com");
  telegramTLS.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  registerRoutes();
  // Les autres capteurs fonctionnent meme si le DS18B20 manque.
  readSensors();
  lastSampleMs=millis();
  lastDiskMs=millis();
  Serial.println("Ouvre l'adresse IP affichee apres connexion Wi-Fi.");
}

void dragonLoop() {
  web.handleClient();
  serviceWifi();
  retrySensors();
  readDS18B20();
  uint32_t now=millis();
  if(now-lastSampleMs>=SAMPLE_MS){
    lastSampleMs=now;
    readSensors();
    checkAlerts();
  }
  calibrationTick();
  if(now-lastDiskMs>=DISK_MS) {
    lastDiskMs=now;
    saveSample();
    Serial.printf("DragonFruit: sol=%d air=%.1fC HR=%.1f%% lux=%.0f DS=%s IP=%s\n",
      current.soil,current.temp,current.rh,current.lux,dsOK?"oui":"attente",WiFi.localIP().toString().c_str());
  }
  telegramPoll();
  serviceDiagnostics();
  delay(2);
}
