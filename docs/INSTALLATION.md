# DragonFruit Monitor V3.0 — guide Arduino (22 novembre 2026)

**Projet :** DragonFruit — Faux Poivrier. ESP32-WROOM-32 / carte `ESP32 Dev Module`.

Ceci est un **nouveau sketch autonome**, et non un correctif appliqué ligne par ligne au sketch V2.2 : le fichier source V2.2 complet n'était pas disponible lors de la création. Il reprend le câblage et les formats CSV connus. La compilation a été vérifiée **syntaxiquement avec un environnement simulé** ; le sketch n'a pas été compilé contre le véritable cœur Arduino-ESP32 ni téléversé sur ton ESP32. La vérification finale est à faire dans Arduino IDE avec tes bibliothèques.

## 1. Ce qui fonctionne dès maintenant dans le programme

- Capteurs déjà connectés : humidité du substrat (ADC GPIO34), AHT20, BMP280 et BH1750 (I²C GPIO21/22).
- DS18B20 **déjà prévu par le code sur GPIO27**, mais entièrement facultatif tant qu'il n'est pas branché : il affiche **« en attente »**, sans interrompre les autres acquisitions.
- Mesures toutes les **5 secondes** ; journal sur LittleFS toutes les **5 minutes** une fois l'horloge synchronisée.
- Interface locale responsive avec cinq onglets, six graphiques, conseils sur l'arrosage/ambiance/lumière et mode sombre selon l'appareil.
- Calibration de stabilité automatique de **5 minutes** et **16 mesures**, lancée depuis Safari sans programme supplémentaire sur le Mac.
- Boutons pour enregistrer les références **avant** et **après** arrosage, avec confirmation. L'ancien repère `1279` est mémorisé comme **référence de position uniquement**, pas comme 100 % d'eau ou seuil d'arrosage.
- Notifications Telegram : alertes prolongées de température/humidité et, après première détection, disparition du DS18B20. Commandes `/status`, `/etat`, `/help`, `/start` et bouton test.
- Analyse du VPD et de l'exposition lumineuse depuis l'historique : **pas** de diagnostic automatique de santé de l'arbre, et **pas** d'alerte lumière basée sur une mesure nocturne.
- Une estimation prudente de la date à laquelle **vérifier** l'arrosage devient possible après les **deux références** et au moins **12 heures de tendance cohérente** dans la nouvelle position (il faut plusieurs cycles pour la rendre pertinente). Elle ne remplace pas l'observation du substrat.

## 2. AVANT de changer le programme : préserver tes historiques

1. Allume ton ancien firmware V2.2, connecte le Mac au même Wi-Fi et ouvre l'adresse de DragonFruit.
2. Télécharge `/export.csv` et `/arrosages.csv` sur le Mac ; garde les fichiers en dehors du dossier Arduino.
3. Dans Arduino IDE, note et **conserve exactement le même schéma de partition** : `Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)` si c'était celui choisi dans V2.2. **Ne sélectionne pas « Erase All Flash », ne formate pas LittleFS et n'utilise pas « upload filesystem image ».**
4. Le nouveau sketch exécute `LittleFS.begin(false)` : il **ne formate pas** le stockage. Les anciens fichiers `/d<jour>.csv` ne sont **jamais modifiés**. Les nouvelles données vont dans `/v3/d<jour>.csv`. L'historique affiche les deux ; l'export produit des colonnes harmonisées.
5. Le stockage n'étant pas illimité, télécharge régulièrement tes CSV. Si la partition est pleine, l'interface signalera une erreur de sauvegarde. Aucune suppression automatique d'anciens fichiers n'est effectuée.

**Ce qui n'est pas garanti sans le sketch V2.2 complet :** l'emplacement physique et le format de tous les anciens fichiers, une migration des anciens réglages `Preferences`, ni la conservation des données si le partitionnement de l'ESP32 a été modifié. Les CSV déjà exportés restent ta sauvegarde indépendante.

## 3. Installation pas à pas

1. Décompresse `DragonFruit_V3.zip`. Ouvre le dossier `DragonFruit_V3`, puis double-clique `DragonFruit_V3.ino` dans **Arduino IDE**. Les fichiers `.cpp` et `.h` doivent **rester dans ce même dossier** : ne copie pas seulement `.ino`.
2. Ouvre l'onglet `secrets.h` et renseigne :

```cpp
#define DRAGON_WIFI_SSID "NOM_DE_TON_WIFI_2_4_GHZ"
#define DRAGON_WIFI_PASSWORD "MOT_DE_PASSE_WIFI"
#define DRAGON_TG_TOKEN "NOUVEAU_TOKEN_GENERE_PAR_BOTFATHER"
#define DRAGON_TG_CHAT_ID "TON_CHAT_ID"
#define DRAGON_LOCAL_CODE "UnCodeLocalSecretDe8CaracteresMinimum"
```

**Important :** ne publie jamais `secrets.h` : il contient tes identifiants. Un token Telegram exposé doit être révoqué depuis BotFather.

3. Menu **Outils → Type de carte → ESP32 Dev Module** ; conserve le port USB que tu utilisais avec V2.2.
4. Menu **Outils → Partition Scheme** : **même valeur** qu'en V2.2. Ne modifie pas cette option pour faire de la place sans sauvegarder et accepter une migration du stockage.
5. Dans le **Gestionnaire de bibliothèques**, vérifie :

   - `BH1750` de Christopher Laws (version actuelle compatible avec `BH1750::CONTINUOUS_HIGH_RES_MODE`)
   - `Adafruit AHTX0`
   - `Adafruit BMP280 Library`
   - `OneWire` **2.3.8 ou plus récent** si tu utilises Arduino-ESP32 3.x
   - `DallasTemperature` de Miles Burton
   - `UniversalTelegramBot` de Brian Lough (et `ArduinoJson` et dépendances)
   - Les bibliothèques `WiFi`, `Wire`, `WebServer`, `LittleFS`, `Preferences` et `ESPmDNS` sont incluses dans le cœur ESP32.

6. Clique sur **Vérifier ✓**. Si la compilation réussit, clique sur **Téléverser**. Le transfert coupe brièvement l'acquisition ; celle-ci reprendra automatiquement après redémarrage. Aucun logiciel n'est capable de conserver une acquisition réellement continue **pendant** le flashage.
7. Ouvre le **Moniteur série, 115200 bauds**. Quand le Wi-Fi se connecte, la carte affiche son IP ; un nom local `http://dragonfruit.local` est également tenté si mDNS est accepté par le réseau.
8. Sur l'iPhone connecté au même Wi-Fi, ouvre `http://dragonfruit.local` ou l'IP affichée dans le moniteur série. Dans Safari → Partager → **Sur l'écran d'accueil** pour avoir une icône comme une application.
9. Ouvre **Réglages → Envoyer un test Telegram**, saisis le code local défini dans `secrets.h` quand la page le demande.
10. Dans **Calibration**, lance le test de cinq minutes. Ne bouge pas la sonde et n'arrose pas pendant le relevé. À la fin, l'application mémorisera la médiane de position (sans déclencher automatiquement un arrosage).

## 4. Mercredi : installer le DS18B20 sans modifier une ligne du programme

Éteins et **débranche le chargeur USB** avant de toucher aux câbles. Le DS18B20 doit être branché ainsi :

| DS18B20 | ESP32 | Notes |
|---|---|---|
| VDD / VCC | 3,3 V | Jamais envoyer 5 V dans les GPIO |
| GND | GND | Masse commune |
| DQ / DATA | GPIO27 | Une résistance **4,7 kΩ entre DQ et 3,3 V** est nécessaire si elle n'est pas déjà intégrée au module |

Seule la **pointe métallique étanche** pénètre dans le substrat ; les jonctions électriques restent au sec. Ne te fie pas aux couleurs avant d'avoir vérifié le brochage du vendeur. Tu peux utiliser une breadboard et des câbles Dupont, sans soudure. Rebranche ensuite l'USB : le firmware détectera le DS18B20 au démarrage et la sixième mesure apparaîtra si la sonde répond. Les cinq autres paramètres continueront de fonctionner même sans elle.

## 5. Utilisation quotidienne

- **Accueil** : mesures, détection des modules, conseils prudents et données récentes.
- **Graphiques** : 1 h / 6 h / 24 h / 7 j / 30 j, six paramètres, anciens relevés V2.2 et nouveaux V3.
- **Entretien** : noter un arrosage réel et consulter/télécharger le journal.
- **Calibration** : stabilité de position (5 minutes) et références avant/après arrosage. Si tu **déplaces encore la sonde**, utilise « Signaler un nouveau déplacement » : cela invalide les références d'arrosage, **sans supprimer les mesures**.
- **Réglages** : seuils de température/humidité, test Telegram, diagnostics, état de la mémoire flash.

**Quand arroser ?** La V3 ne décidera pas à ta place tant que `wetRaw` et `dryRaw` ne sont pas confirmés. Lorsque la surface du substrat commence réellement à sécher et qu'un arrosage est nécessaire, clique **« Référence avant arrosage »** avant d'arroser. Après arrosage et 30–60 minutes d'égouttage, clique **« Référence après égouttage »**. Confirme l'action par une observation manuelle. Plusieurs cycles seront utiles pour fiabiliser la prévision.

**Lumière :** la V3 analyse les lux et les lux·h, uniquement comme **repères comparatifs**. Le BH1750 doit être placé de façon représentative du feuillage, sinon il mesure surtout la lumière reçue par la breadboard. Les lux·h ne sont pas des DLI photosynthétiques. Le programme n'allume pas physiquement ta lampe : sa minuterie reste indépendante.

## 6. Limites connues et sécurité

- L'interface est **locale en HTTP** ; ne publie **jamais** le port 80 de l'ESP32 sur Internet. Le code de contrôle protège les actions sur le réseau local mais HTTP ne chiffre pas son transport. Le bot Telegram sert pour les alertes à distance.
- Les données ne sont pas envoyées automatiquement vers Google Sheets dans **V3.0** ; une sauvegarde externe peut faire partie d'une V3.1 ultérieure. Ne confonds pas la récupération possible des historiques V2.2 et une sauvegarde cloud.
- Pas de notifications automatiques sur la lumière avant d'avoir validé le placement du BH1750 et analysé plusieurs journées. Le commutateur d'alerte lumineuse est volontairement inactif dans l'interface V3.0.
- Une coupure électrique entraîne une interruption inévitable ; après redémarrage, les anciennes mesures sauvegardées demeurent sur LittleFS si le partitionnement est inchangé. **Sans Wi-Fi / synchronisation NTP après un redémarrage**, les nouveaux relevés continuent en RAM mais ne sont pas datés ni écrits sur flash tant que l'horloge n'est pas valide.
- La V3 ne peut ni détecter une maladie, ni mesurer le pourcentage absolu d'eau du pot avec une calibration uniquement par deux points. L'« indice relatif » n'est pas une mesure d'humidité volumique.
- Le firmware et le front-end ont été vérifiés **syntaxiquement**, sans essai final sur le matériel ni compilation contre toutes les bibliothèques : un ajustement peut être nécessaire selon la version Arduino-ESP32 et les bibliothèques installées. En cas d'erreur, copie le **premier message d'erreur du compilateur**.

**Fichiers essentiels :** `DragonFruit_V3.ino`, `dragon_core.cpp`, `dragon_core.h`, `config.h`, `secrets.example.h`, `page.h` (ainsi que `secrets.h`, à créer uniquement en local). Ils doivent rester ensemble dans le dossier `DragonFruit_V3`.
