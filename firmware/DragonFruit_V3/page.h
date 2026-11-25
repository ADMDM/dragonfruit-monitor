#pragma once
// Page entierement autonome, pas d'acces CDN et pas de chargement externe.
static const char DRAGON_PAGE[] PROGMEM = R"DRAGON(
<!DOCTYPE html>
<html lang="fr">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#183f32">
<meta name="apple-mobile-web-app-capable" content="yes">
<meta name="apple-mobile-web-app-title" content="DragonFruit">
<title>DragonFruit · Faux Poivrier</title>
<style>
:root{color-scheme:light;--ink:#1b3329;--muted:#748477;--line:#e0e8e0;--bg:#f4f6f0;--card:#fff;--accent:#237a56;--forest:#153d30;--gold:#e0aa4c;--danger:#a54736;--shadow:0 14px 42px rgba(28,56,40,.055)}
*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;background:var(--bg);color:var(--ink);font-family:ui-sans-serif,-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;line-height:1.45}
button,input,select{font:inherit}button{cursor:pointer}button:disabled{opacity:.5;cursor:not-allowed}a{color:var(--accent)}.wrap{max-width:1100px;margin:auto;padding:0 22px}.top{background:linear-gradient(125deg,#153d30,#285b44);color:#fff;position:relative;overflow:hidden}.top:after{content:"";position:absolute;right:-95px;top:-110px;width:350px;height:350px;border-radius:50%;border:55px solid #ffffff08;pointer-events:none}.topbar{height:80px;display:flex;align-items:center;justify-content:space-between;gap:15px}.brand{font-size:16px;letter-spacing:.5px;font-weight:800;display:flex;align-items:center;gap:12px}.brandIcon{width:36px;height:36px;background:#e3f1df25;border:1px solid #a6d6b44d;border-radius:11px;display:flex;align-items:center;justify-content:center}.on{display:flex;gap:7px;align-items:center;font-size:12px;color:#bfe5c9}.dot{width:8px;height:8px;border-radius:50%;background:#75dfa0;box-shadow:0 0 10px #8be5a188}.dot.off{background:#dcb66c;box-shadow:none}.head{padding:26px 0 84px;display:grid;grid-template-columns:1.4fr .6fr;gap:22px;align-items:center}.eyebrow{font-size:11px;color:#b0d8ba;letter-spacing:2.2px;text-transform:uppercase;font-weight:750}.head h1{font-size:clamp(38px,5vw,57px);letter-spacing:-2px;margin:8px 0 3px;font-weight:800}.subtitle{color:#c8e5d1;font-size:16px;margin:0}.meta{margin-top:25px;display:flex;flex-wrap:wrap;gap:10px}.chip{font-size:12px;color:#deeee4;border:1px solid #ffffff38;padding:8px 12px;border-radius:100px;background:#ffffff0c}.tree{justify-self:end;width:160px;height:160px;opacity:.8}.container{position:relative;margin-top:-55px;padding-bottom:100px}.panel{display:none}.panel.active{display:block}.nav{display:flex;gap:7px;padding:7px;border-radius:17px;background:#fff;box-shadow:var(--shadow);border:1px solid var(--line);margin-bottom:25px;position:sticky;top:8px;z-index:8;overflow-x:auto}.nav button{border:0;background:transparent;color:#6c7a70;border-radius:11px;display:flex;align-items:center;gap:8px;font-size:13px;padding:12px 18px;white-space:nowrap;font-weight:720}.nav button[aria-selected="true"]{background:#e5f1e8;color:var(--accent)}.section{display:flex;align-items:end;justify-content:space-between;gap:14px;margin:14px 0 15px}.section h2{font-size:22px;letter-spacing:-.5px;margin:0}.section p{margin:4px 0 0;color:var(--muted);font-size:13px}.metrics{display:grid;grid-template-columns:repeat(3,minmax(0,1fr));gap:13px}.metric,.card{background:var(--card);border:1px solid var(--line);box-shadow:var(--shadow);border-radius:18px}.metric{padding:20px;min-height:142px;position:relative;overflow:hidden}.metric:before{content:"";position:absolute;left:0;top:0;bottom:0;width:3px;background:var(--stripe,#65ad83)}.metric .label{font-weight:650;color:#728377;font-size:12px;display:flex;gap:8px;align-items:center}.metric .num{font-size:clamp(24px,3vw,32px);letter-spacing:-1px;font-weight:820;margin:14px 0 4px;font-variant-numeric:tabular-nums}.metric .hint{font-size:11px;color:#879387}.icon{font-size:17px;font-weight:normal}.card{padding:21px}.card h3{font-size:16px;margin:0 0 10px;letter-spacing:-.25px}.card p{margin:7px 0 0;font-size:13px;color:#62786b}.advice{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:13px;margin-top:14px}.advice .card{min-height:148px}.adviceHeader{display:flex;align-items:center;gap:10px;font-weight:800;font-size:14px}.adviceIcon{display:flex;width:35px;height:35px;border-radius:11px;background:#ebf4ec;align-items:center;justify-content:center;font-size:17px}.pill{font-size:11px;background:#eaf2e9;color:#376e4f;border-radius:100px;padding:6px 9px;display:inline-block;font-weight:750}.pill.neutral{color:#796a40;background:#f5f0df}.pill.warn{color:#96513a;background:#fbede6}.miniInfo{margin-top:12px;padding:13px 14px;background:#ecf3ec;border-radius:11px;color:#456950;font-size:12px}.chartGrid{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:14px}.chart{min-width:0}.chart canvas{display:block;height:205px;width:100%;touch-action:pan-y}.chart .metricMeta{display:flex;justify-content:space-between;color:#7a887d;font-size:11px;margin-top:12px}.ranges{display:flex;gap:6px;flex-wrap:wrap}.ranges button,.secondary,.primary,.outline{border-radius:10px;border:1px solid #dce7dd;padding:10px 13px;font-weight:750;font-size:12px;background:#fff;color:#476655}.ranges button.selected{background:#236e4e;color:white;border-color:#236e4e}.primary{background:var(--accent);color:white;border-color:var(--accent)}.primary:hover{background:#195c40}.outline{border-color:#e3d4c5;color:#96553d}.secondary{background:#edf4ee;color:#306d4f}.actions{display:flex;gap:9px;flex-wrap:wrap;margin-top:17px}.form{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:14px}.field{display:flex;flex-direction:column;gap:6px}.field label{font-size:12px;color:#667a6c;font-weight:750}.field input{width:100%;border:1px solid #dce5dc;border-radius:11px;background:#fcfdfa;padding:11px 12px;min-height:43px;color:var(--ink)}.field input[type=checkbox]{width:auto;min-height:0;accent-color:var(--accent)}.checkline{display:flex;gap:10px;align-items:center;font-size:13px}.full{grid-column:1/-1}.divider{height:1px;background:var(--line);margin:20px 0}.step{display:flex;gap:13px;margin-bottom:18px;align-items:flex-start}.stepnum{flex:none;width:29px;height:29px;border-radius:50%;background:#e6f2e9;display:grid;place-items:center;color:#28754c;font-weight:800;font-size:12px}.step b{font-size:13px}.step p{margin:4px 0 0}.bar{background:#e5eee6;height:10px;overflow:hidden;border-radius:99px;margin:12px 0}.bar span{display:block;height:100%;background:#2d9467;width:0;transition:width .4s}.alertBox{padding:15px 17px;border:1px solid #e9e3ce;background:#faf7ec;color:#746036;border-radius:12px;font-size:13px}.error{color:#a54736}.good{color:#247852}.subtle{font-size:12px;color:var(--muted)}.two{display:grid;grid-template-columns:1fr 1fr;gap:14px}.diag{display:grid;grid-template-columns:1fr auto;gap:8px 14px;font-size:12px}.diag dt{color:#728477}.diag dd{margin:0;font-weight:700;text-align:right}.log{display:grid;gap:8px;margin-top:10px}.event{display:flex;justify-content:space-between;gap:10px;padding:10px 0;border-bottom:1px solid var(--line);font-size:12px}.event:last-child{border:0}.toast{position:fixed;bottom:25px;left:50%;transform:translateX(-50%);background:#183d30;color:#fff;padding:12px 18px;border-radius:12px;box-shadow:0 10px 30px #173c3066;z-index:100;max-width:min(90vw,500px);text-align:center;font-size:13px;display:none}.toast.visible{display:block}.foot{text-align:center;color:#879587;font-size:11px;margin-top:35px}.hide{display:none!important}
@media(max-width:760px){.wrap{padding:0 14px}.topbar{height:70px}.head{padding:18px 0 80px;grid-template-columns:1fr 90px}.tree{width:90px;height:90px}.meta{gap:6px}.chip{padding:6px 9px}.metrics{grid-template-columns:repeat(2,minmax(0,1fr));gap:10px}.metric{padding:16px;min-height:120px}.metric .num{font-size:25px;margin:9px 0 3px}.advice,.chartGrid,.two{grid-template-columns:1fr}.card{padding:17px}.nav{position:fixed;bottom:0;top:auto;left:0;right:0;margin:0;border-radius:16px 16px 0 0;border-bottom:0;justify-content:space-around;padding:8px max(5px,env(safe-area-inset-right)) calc(8px + env(safe-area-inset-bottom));overflow:visible;box-shadow:0 -6px 22px #0d2f2216}.nav button{flex-direction:column;gap:4px;padding:6px 5px;font-size:10px;min-width:62px}.container{padding-bottom:130px}.section h2{font-size:20px}.form{grid-template-columns:1fr}.toast{bottom:94px}}
@media(prefers-color-scheme:dark){:root{color-scheme:dark;--ink:#edf3ed;--muted:#9bb7a5;--line:#344f3f;--bg:#101d18;--card:#203328;--accent:#73ca94;--forest:#173d30;--shadow:none}body{background:var(--bg)}.metric .label,.metric .hint,.card p,.section p,.chart .metricMeta{color:#acc2b0}.metric .num{color:#f4f7f1}.nav{background:#203328}.nav button[aria-selected="true"]{background:#305542;color:#c5f2d0}.pill{background:#355740;color:#b8e6c1}.miniInfo,.adviceIcon,.stepnum{background:#2d4434;color:#c0ecc8}.field input{background:#1a2d23;border-color:#49634e;color:#eaf6ee}.alertBox{background:#3d3321;border-color:#68563a;color:#e4cca1}.secondary,.ranges button{background:#2b4937;border-color:#48634b;color:#d2edd8}.ranges button.selected{background:#2a8659;color:white}.outline{background:#463329;border-color:#80553f;color:#f2bea4}.diag dt{color:#a9bdac}}

/* Graphes interactifs, utilisables au toucher, sans dependance externe. */
.graphControls{display:flex;align-items:center;flex-wrap:wrap;gap:7px;margin:12px 0 16px}
.graphControls button{border:1px solid var(--line);background:var(--card);color:var(--ink);padding:9px 12px;border-radius:10px;font-size:12px;font-weight:700}
.graphControls button:disabled{opacity:.35}
.graphControls .selectedRange{font-size:11px;color:var(--muted);margin-left:auto}
.chart canvas{cursor:crosshair;touch-action:pan-y}
.chart .pointDetail{min-height:36px;line-height:1.5;margin-top:12px;border-radius:9px;background:var(--bg);padding:9px 12px;color:var(--ink);font-size:12px;font-weight:620}
.chart .graphTip{margin:7px 0 2px;font-size:11px;color:var(--muted)}
@media(max-width:760px){.graphControls .selectedRange{width:100%;margin-left:0}.chart canvas{height:245px}}
</style>
</head>
<body>
<header class="top"><div class="wrap">
 <div class="topbar"><div class="brand"><span class="brandIcon">✦</span> DragonFruit <span style="font-weight:400;color:#b9d5c2;font-size:11px">MONITOR V3.1</span></div><div class="on"><span class="dot off" id="dot"></span><span id="connection">Connexion…</span></div></div>
 <div class="head"><div><div class="eyebrow">Mon jardin connecté</div><h1>DragonFruit</h1><p class="subtitle">Faux Poivrier · Suivi intelligent</p><div class="meta"><span class="chip" id="heroTemp">Température —</span><span class="chip" id="heroHr">Humidité —</span><span class="chip" id="heroTime">Mesure en attente</span></div></div>
  <svg class="tree" viewBox="0 0 150 150" aria-hidden="true"><path d="M75 123Q70 90 84 67M77 100Q57 85 45 69M78 84Q101 72 108 53" fill="none" stroke="#d9e2bc" stroke-width="7" stroke-linecap="round"/><g fill="#a8d5aa"><ellipse cx="43" cy="57" rx="27" ry="14" transform="rotate(-40 43 57)"/><ellipse cx="76" cy="53" rx="27" ry="17" transform="rotate(23 76 53)"/><ellipse cx="107" cy="46" rx="25" ry="14" transform="rotate(-24 107 46)"/><ellipse cx="59" cy="79" rx="18" ry="12" transform="rotate(20 59 79)"/></g><path d="M51 125H105L98 144H57Z" fill="#a68457"/></svg>
 </div>
</div></header>
<main class="wrap container">
<nav class="nav" role="tablist" aria-label="Navigation DragonFruit">
  <button role="tab" aria-selected="true" data-tab="home"><span class="icon">⌂</span> Accueil</button>
  <button role="tab" aria-selected="false" data-tab="graphs"><span class="icon">▥</span> Graphiques</button>
  <button role="tab" aria-selected="false" data-tab="care"><span class="icon">♧</span> Entretien</button>
  <button role="tab" aria-selected="false" data-tab="calibration"><span class="icon">⚙</span> Calibration</button>
  <button role="tab" aria-selected="false" data-tab="settings"><span class="icon">☷</span> Réglages</button>
</nav>
<section class="panel active" id="home">
  <div class="section"><div><h2>Vue d'ensemble</h2><p>Conditions mesurées autour de DragonFruit</p></div><span class="pill" id="fresh">En attente</span></div>
  <div class="metrics">
    <article class="metric" style="--stripe:#62b48b"><div class="label"><span class="icon">◉</span> Humidité du substrat</div><div class="num" id="vSoil">—</div><div class="hint" id="soilHint">Valeur brute · en attente de calibration</div></article>
    <article class="metric" style="--stripe:#db996e"><div class="label"><span class="icon">♨</span> Température de l'air</div><div class="num" id="vTemp">—</div><div class="hint">AHT20</div></article>
    <article class="metric" style="--stripe:#74aac7"><div class="label"><span class="icon">≋</span> Humidité de l'air</div><div class="num" id="vRh">—</div><div class="hint">Humidité relative AHT20</div></article>
    <article class="metric" style="--stripe:#dfb55c"><div class="label"><span class="icon">☀</span> Luminosité</div><div class="num" id="vLux">—</div><div class="hint">Capteur BH1750</div></article>
    <article class="metric" style="--stripe:#8b9bcb"><div class="label"><span class="icon">♧</span> Température des racines</div><div class="num" id="vRoot">—</div><div class="hint" id="rootHint">DS18B20 · pas encore branché</div></article>
    <article class="metric" style="--stripe:#a7aaa7"><div class="label"><span class="icon">◌</span> Pression atmosphérique</div><div class="num" id="vPress">—</div><div class="hint">BMP280 · hPa</div></article>
  </div>
  <div class="section" style="margin-top:30px"><div><h2>Les conseils pour DragonFruit</h2><p>Des actions raisonnées, pas un score de santé inventé</p></div></div>
  <div class="advice">
   <article class="card"><div class="adviceHeader"><span class="adviceIcon">♧</span> Arrosage</div><p id="soilAdvice">Nous attendons les mesures de la station.</p><div class="miniInfo" id="prediction">Prévision : en attente de deux références réelles.</div></article>
   <article class="card"><div class="adviceHeader"><span class="adviceIcon">☀</span> Éclairage</div><p id="lightAdvice">Il faut une journée de mesures au niveau des feuilles pour interpréter la lumière.</p><div class="miniInfo" id="lightSum">Historique en cours d'analyse.</div></article>
   <article class="card"><div class="adviceHeader"><span class="adviceIcon">♨</span> Ambiance</div><p id="airAdvice">Analyse de la température et de l'humidité.</p><div class="miniInfo" id="vpd">VPD : —</div><div class="miniInfo" id="humidityAdvice">Brumisation systématique non recommandée : contrôler d’abord l'humidité de la pièce.</div></article>
   <article class="card"><div class="adviceHeader"><span class="adviceIcon">◈</span> Diagnostic</div><p id="healthAdvice">État de l'ESP32 et des capteurs.</p><div class="miniInfo" id="nextAction">Actualisation automatique toutes les 4 secondes.</div></article>
  </div>
</section>
<section class="panel" id="graphs">
  <div class="section"><div><h2>Graphiques & tendances</h2><p>Choisis une période et compare les paramètres</p></div></div>
  <div class="ranges" role="group" aria-label="Période de l'historique">
   <button class="selected" data-hours="24">24 h</button><button data-hours="1">1 h</button><button data-hours="6">6 h</button><button data-hours="168">7 j</button><button data-hours="720">30 j</button>
  </div>
   <div class="graphControls" aria-label="Explorer les courbes">
    <button type="button" id="graphEarlier" title="Se déplacer vers les mesures plus anciennes">← Plus tôt</button>
    <button type="button" id="graphZoomIn" title="Zoomer sur la période">Zoom +</button>
    <button type="button" id="graphZoomOut" title="Dézoomer">Zoom −</button>
    <button type="button" id="graphLater" title="Se déplacer vers les mesures récentes">Plus récent →</button>
    <button type="button" id="graphReset">Réinitialiser</button>
    <span class="selectedRange" id="graphRange">Touchez une courbe pour lire les mesures exactes.</span>
   </div>
  <div class="chartGrid" id="chartGrid">
   <article class="card chart"><h3>Substrat · valeur brute</h3><canvas data-series="soil" aria-label="Graphique du substrat"></canvas><div class="pointDetail" id="detail-soil" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>ADC 0–4095</span><span id="stat-soil">—</span></div></article>
   <article class="card chart"><h3>Température de l'air</h3><canvas data-series="temperature" aria-label="Graphique de température"></canvas><div class="pointDetail" id="detail-temperature" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>°C</span><span id="stat-temperature">—</span></div></article>
   <article class="card chart"><h3>Humidité de l'air</h3><canvas data-series="humidity" aria-label="Graphique d'humidité"></canvas><div class="pointDetail" id="detail-humidity" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>% HR</span><span id="stat-humidity">—</span></div></article>
   <article class="card chart"><h3>Luminosité</h3><canvas data-series="lux" aria-label="Graphique d'éclairage"></canvas><div class="pointDetail" id="detail-lux" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>lux</span><span id="stat-lux">—</span></div></article>
   <article class="card chart"><h3>Pression atmosphérique</h3><canvas data-series="pressure" aria-label="Graphique de pression"></canvas><div class="pointDetail" id="detail-pressure" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>hPa</span><span id="stat-pressure">—</span></div></article>
   <article class="card chart"><h3>Température du substrat</h3><canvas data-series="soilTemperature" aria-label="Graphique température racines"></canvas><div class="pointDetail" id="detail-soilTemperature" role="status">Touchez ou déplacez le curseur sur une courbe pour lire la valeur et l’heure exacte.</div><div class="metricMeta"><span>°C · DS18B20</span><span id="stat-soilTemperature">—</span></div></article>
    <article class="card chart"><h3>Comparer l'air et les racines</h3><canvas data-series="compareTemp" aria-label="Températures air et racines" ></canvas><div class="pointDetail" id="detail-compareTemp" role="status">Touchez la courbe pour comparer la température de l'air et celle des racines.</div><div class="metricMeta"><span>Air : orange · racines : vert</span><span id="stat-compareTemp">—</span></div></article>
   </div>
   <div class="actions"><a class="secondary" id="exportPeriod" href="/export.csv?hours=24" download>Exporter 24 h (CSV complet)</a></div><p class="subtle">Le CSV contient toutes les mesures enregistrées sur la période choisie, même si tu as zoomé sur les graphiques.</p>
  <div class="miniInfo">Les anciennes mesures V2.2 sont affichées avec les nouvelles V3. Les variations après déplacement de la sonde ne doivent pas être interprétées comme un arrosage.</div>
</section>
<section class="panel" id="care">
  <div class="section"><div><h2>Journal d'entretien</h2><p>Conserver des observations concrètes pour l'arrosage</p></div></div>
  <div class="two">
   <article class="card"><h3>Arrosage de DragonFruit</h3><p>N'appuie sur ce bouton qu'après avoir réellement arrosé. L'eau doit pouvoir s'écouler et les raccordements rester au sec.</p><div class="actions"><button class="primary" id="waterBtn">J'ai arrosé DragonFruit</button></div><p class="subtle">Le bouton conserve la date et l'heure dans le journal existant /arrosages.csv.</p></article>
   <article class="card"><h3>Prochain arrosage</h3><p id="careAdvice">Pas de décision automatique avant calibration. Vérifie manuellement le substrat.</p><div class="miniInfo">Les prévisions seront indicatives et nécessiteront plusieurs cycles cohérents.</div></article>
  </div>
  <article class="card" style="margin-top:14px"><h3>Derniers arrosages enregistrés</h3><div class="log" id="waterLog">Chargement du journal…</div><div class="actions"><a class="secondary" href="/arrosages.csv" download>Télécharger le journal CSV</a></div></article>
</section>
<section class="panel" id="calibration">
  <div class="section"><div><h2>Étalonnage du substrat</h2><p>La sonde ne doit pas bouger pendant les mesures</p></div></div>
  <div class="two">
   <article class="card"><h3>Relevé automatique de 5 minutes</h3><div class="step"><span class="stepnum">1</span><div><b>Conserver la nouvelle position</b><p>Ne déplace plus la sonde et n'arrose pas pendant le test.</p></div></div><div class="step"><span class="stepnum">2</span><div><b>16 mesures toutes les 20 secondes</b><p>La station mesure automatiquement, même si tu fermes Safari.</p></div></div><div class="step"><span class="stepnum">3</span><div><b>Enregistrer une référence stable</b><p>Ce résultat n'est pas un pourcentage d'humidité.</p></div></div><div class="actions"><button class="primary" id="calStartBtn">Démarrer le relevé</button></div><div class="bar"><span id="calBar"></span></div><div class="subtle" id="calProgress">En attente · 0/16 mesures</div><div class="miniInfo" id="calResult">Dernier repère expérimental : 1279.</div></article>
   <article class="card"><h3>Calibration du véritable arrosage</h3><p>Quand le substrat est réellement prêt à être arrosé, enregistre une référence « avant ». Après arrosage et égouttage, enregistre la référence « après ».</p><div class="actions"><button class="secondary" id="beforeBtn">Référence avant arrosage</button><button class="secondary" id="afterBtn">Référence après égouttage</button></div><div class="divider"></div><div id="refs">Références en attente.</div><div class="divider"></div><p><b>Sonde déplacée ?</b> Ce bouton invalide les références avant/après arrosage. Il ne supprime pas les mesures existantes.</p><button class="outline" id="movedBtn">Signaler un nouveau déplacement</button></article>
  </div>
</section>
<section class="panel" id="settings">
  <div class="section"><div><h2>Réglages & diagnostic</h2><p>Les paramètres sont conservés après redémarrage de l'ESP32</p></div></div>
  <div class="two">
   <article class="card"><h3>Conditions et alertes</h3><form class="form" id="settingsForm">
    <div class="field"><label for="tLow">Alerte froid (°C)</label><input type="number" step="0.1" name="tLow" id="tLow" required></div>
    <div class="field"><label for="tHigh">Alerte chaleur (°C)</label><input type="number" step="0.1" name="tHigh" id="tHigh" required></div>
    <div class="field"><label for="rhLow">Air sec (% HR)</label><input type="number" step="0.1" name="rhLow" id="rhLow" required></div>
    <div class="field"><label for="rhHigh">Air humide (% HR)</label><input type="number" step="0.1" name="rhHigh" id="rhHigh" required></div>
    <div class="field"><label for="lightLow">Repère lumière (lux)</label><input type="number" step="100" name="lightLow" id="lightLow" required></div>
    <div class="field"><label>Notifications</label><span class="checkline"><input type="checkbox" name="telegram" id="telegramOn"> Telegram actif</span></div>
    <div class="field full"><label>Analyse lumière</label><span class="checkline"><input type="checkbox" name="lightAlerts" id="lightAlerts" disabled> Alerte automatique après plusieurs journées (à venir)</span><div class="subtle">L'analyse est déjà affichée. Pas d'alerte Telegram sur un simple lux ponctuel.</div></div>
    <div class="full"><button class="primary" type="submit">Sauvegarder les réglages</button></div>
   </form><div class="actions"><button class="secondary" id="telegramTestBtn">Envoyer un test Telegram</button></div></article>
   <article class="card"><h3>Station ESP32</h3><dl class="diag"><dt>Wi-Fi</dt><dd id="dWifi">—</dd><dt>Adresse IP</dt><dd id="dIp">—</dd><dt>Horloge</dt><dd id="dClock">—</dd><dt>Dernière sauvegarde</dt><dd id="dSaved">—</dd><dt>Diagnostic stockage</dt><dd id="dStorage">—</dd><dt>Dernier redémarrage</dt><dd id="dReset">—</dd><dt>RAM disponible</dt><dd id="dHeap">—</dd><dt>Moniteur extérieur</dt><dd id="dHeartbeat">—</dd><dt>LittleFS</dt><dd id="dFs">—</dd><dt>AHT20</dt><dd id="dAht">—</dd><dt>BMP280</dt><dd id="dBmp">—</dd><dt>BH1750</dt><dd id="dBh">—</dd><dt>DS18B20</dt><dd id="dDs">En attente</dd><dt>Telegram</dt><dd id="dTg">—</dd></dl><div class="divider"></div><p>La V3 écrit dans <code>/v3/</code> et laisse intacts les anciens fichiers V2.2. Sauvegarde les exports avant chaque mise à jour.</p><div class="miniInfo">Le serveur fonctionne sur le réseau local. Les notifications Telegram fonctionnent aussi hors de chez toi si l'ESP32 reste connecté à Internet.</div></article>
  </div>
 </section>
 <div class="foot">DragonFruit Monitor V3.1 · Mesures environnementales, pas diagnostic médical ou botanique · ESP32</div>
</main>
<div class="toast" id="toast" role="status" aria-live="polite"></div>
<script>
'use strict';
const $=id=>document.getElementById(id);
const val=(v,unit='',digits=1)=>v==null || !Number.isFinite(Number(v)) ? '—' : Number(v).toLocaleString('fr-BE',{maximumFractionDigits:digits})+(unit?' '+unit:'');
let state=null, hours=24, history=[], live=[], tab='home', busyHistory=false, settingsDirty=false;
let waterTimes=[],graphZoom=1,graphPan=0,graphCursor=null;
function toast(message){const e=$('toast');e.textContent=message;e.classList.add('visible');clearTimeout(toast.t);toast.t=setTimeout(()=>e.classList.remove('visible'),4500);}
function stamp(s){if(!s || s<1700000000)return 'Heure non synchronisée';return new Date(s*1000).toLocaleString('fr-BE',{dateStyle:'short',timeStyle:'short'});}
function chooseTab(t){tab=t;document.querySelectorAll('.panel').forEach(p=>p.classList.toggle('active',p.id===t));document.querySelectorAll('[data-tab]').forEach(b=>b.setAttribute('aria-selected',String(b.dataset.tab===t)));if(t==='graphs'){loadHistory();loadWater();}if(t==='care')loadWater();}
document.querySelectorAll('[data-tab]').forEach(b=>b.addEventListener('click',()=>chooseTab(b.dataset.tab)));
document.querySelectorAll('[data-hours]').forEach(b=>b.addEventListener('click',()=>{hours=Number(b.dataset.hours);graphZoom=1;graphPan=0;graphCursor=null;document.querySelectorAll('[data-hours]').forEach(x=>x.classList.toggle('selected',x===b));setExportLink();loadHistory();}));
const fetchJson=async(url,opts)=>{let r=await fetch(url,{cache:'no-store',...opts});let body=await r.json();if(!r.ok)throw Error(body.message||('Erreur HTTP '+r.status));return body;};
async function post(url,params={}){
 const key=sessionStorage.getItem('dragonfruit-code')||prompt('Code de contrôle défini dans secrets.h :');
 if(!key)throw Error('Code de contrôle nécessaire');
 sessionStorage.setItem('dragonfruit-code',key);
 const r=await fetch(url,{method:'POST',headers:{'X-Dragon-Key':key,'Content-Type':'application/x-www-form-urlencoded'},body:new URLSearchParams(params)});
 const data=await r.json();if(!r.ok){if(r.status===403)sessionStorage.removeItem('dragonfruit-code');throw Error(data.message||'Action refusée');}return data;
}
async function action(url,params={},ask=''){
 if(ask&&!confirm(ask))return false;
 try{let result=await post(url,params);toast(result.message||'Action enregistrée');await loadStatus();return true;}
 catch(e){toast(e.message);return false;}
}
function showAdvice(s){
 const c=s.config||{},raw=s.soil;
 $('soilAdvice').textContent=s.advice||'Mesure du substrat indisponible.';
 $('careAdvice').textContent=$('soilAdvice').textContent;
 $('soilHint').textContent=c.wetRaw && c.dryRaw && c.dryRaw>c.wetRaw+30?'Références enregistrées · indicateur relatif':'Valeur analogique brute · calibration arrosage incomplète';
 let pred='Prévision : nécessite plusieurs cycles cohérents.';
 if(c.wetRaw>0&&c.dryRaw>c.wetRaw+30&&Number.isFinite(raw)){
   const rel=Math.max(0,Math.min(100,100*(c.dryRaw-raw)/(c.dryRaw-c.wetRaw)));
   pred='Indice relatif : '+Math.round(rel)+' % entre deux états observés (pas % d’eau réel).';
   if(s.daysToCheck!=null && Number.isFinite(Number(s.daysToCheck))) pred+=' Vérification estimée dans '+val(s.daysToCheck,'jour(s)',1)+' selon la tendance observée (estimation).';
 }
 $('prediction').textContent=pred;
 const air=Number(s.temperature),rh=Number(s.humidity);
 if(s.temperature==null || s.humidity==null)$('airAdvice').textContent='Capteur AHT20 indisponible. Vérifie le module et ses câbles.';
 else if(air<c.tLow||air>c.tHigh)$('airAdvice').textContent='Température hors de la plage configurée. Vérifie le placement de DragonFruit.';
 else if(rh<c.rhLow)$('airAdvice').textContent='Air sec : vérifie le placement de l’AHT20. Éloigne DragonFruit du chauffage et privilégie un humidificateur d’air si la sécheresse persiste.';
 else if(rh>c.rhHigh)$('airAdvice').textContent='Humidité ambiante élevée. Vérifie la ventilation et les gouttes sur les feuilles.';
 else $('airAdvice').textContent='Température et humidité de l’air dans les plages choisies. Pas de correction immédiate.';
 if(s.temperature!=null&&s.humidity!=null){const e=.6108*Math.exp(17.27*air/(air+237.3))*(1-rh/100);$('vpd').textContent='Demande évaporative de l’air (VPD) : '+val(e,'kPa',2)+' · indicateur, pas ordre d’arroser';}
 else $('vpd').textContent='VPD indisponible sans température et humidité.';
 let missing=[];if(!s.ahtOK)missing.push('AHT20');if(!s.bmpOK)missing.push('BMP280');if(!s.bhOK)missing.push('BH1750');if(s.dsEver&&!s.dsOK)missing.push('DS18B20');
 $('healthAdvice').textContent=s.storageFault?'Attention : nouvelle sauvegarde impossible. Vérifie la mémoire LittleFS et la connexion.':missing.length?'Capteur(s) à vérifier : '+missing.join(', ')+'.':s.fs?'Station et capteurs déjà installés accessibles.':'Stockage LittleFS indisponible : historiques non sauvegardés.';
 $('nextAction').textContent=s.dsOK?'DS18B20 détecté : température des racines suivie.':'Le DS18B20 reste facultatif jusqu’à son installation.';
}
function populateSettings(s){if(settingsDirty)return;const c=s.config||{};for(const k of ['tLow','tHigh','rhLow','rhHigh','lightLow'])$(k).value=c[k]??'';$('telegramOn').checked=!!c.telegram;$('lightAlerts').checked=!!c.lightAlerts;}
function render(s){
 state=s;
 $('dot').classList.toggle('off',!s.wifi);$('connection').textContent=s.wifi?'Station connectée':'Wi-Fi absent';
 $('fresh').textContent=s.clock&&s.epoch&&s.lastMeasurement&&s.epoch-s.lastMeasurement<20?'Mesures récentes':'Mesures à vérifier';
 $('heroTime').textContent=s.lastMeasurement?'Mesure : '+stamp(s.lastMeasurement):'En attente de mesure';
 $('heroTemp').textContent='Air '+val(s.temperature,'°C');$('heroHr').textContent='HR '+val(s.humidity,'%');
 $('vSoil').textContent=s.soil>=0?String(s.soil):'—';$('vTemp').textContent=val(s.temperature,'°C');$('vRh').textContent=val(s.humidity,'%');
 $('vLux').textContent=val(s.lux,'lux',0);$('vRoot').textContent=val(s.soilTemperature,'°C');$('vPress').textContent=val(s.pressure,'hPa',1);
 $('rootHint').textContent=s.dsOK?'DS18B20 · température substrat':s.dsEver?'DS18B20 · vérifier la connexion':'DS18B20 · prévu mercredi';
 $('dWifi').textContent=s.wifi?'Connecté · '+s.rssi+' dBm':'Déconnecté';$('dIp').textContent=s.ip||'—';
 $('dClock').textContent=s.clock?'Synchronisée':'En attente';$('dSaved').textContent=stamp(s.lastSaved);
 $('dStorage').textContent=s.storageFault?'ALERTE : sauvegarde interrompue':s.lastSaveOK?'Écriture déclarée OK':'Échec à vérifier';
 $('dReset').textContent=s.resetReason??'—';$('dHeap').textContent=s.freeHeap?Math.round(s.freeHeap/1024)+' Ko':'—';
 $('dHeartbeat').textContent=s.lastHeartbeatHttp===200?'Dernier ping reçu':s.lastHeartbeatHttp?'Code HTTP '+s.lastHeartbeatHttp:'Non actif / non confirmé';
 $('dFs').textContent=s.fs?Math.round(s.fsUsed/1024)+' / '+Math.round(s.fsTotal/1024)+' Ko'+(s.lastSaveOK?'':' · erreur écriture'):'NON MONTÉ';
 $('dAht').textContent=s.ahtOK?'OK':'Absent';$('dBmp').textContent=s.bmpOK?'OK':'Absent';$('dBh').textContent=s.bhOK?'OK':'Absent';
 $('dDs').textContent=s.dsOK?'OK':s.dsEver?'Absent · vérifier':'En attente';$('dTg').textContent=s.telegramConfigured?'Configuré':'Token absent';
 const c=s.calibration;$('calStartBtn').disabled=!!c.running;$('calProgress').textContent=c.running?'En cours · '+c.count+'/16 mesures':c.done?'Terminé · 16/16 mesures':'Prêt · 0/16 mesures';
 $('calBar').style.width=(c.running?c.count/16:c.done?1:0)*100+'%';
 $('calResult').textContent=c.done?'Médiane '+c.median+' · amplitude '+(c.max-c.min)+' points · référence de position uniquement.':'Repère enregistré : '+s.config.positionRaw+' · pas un pourcentage d’eau.';
 $('refs').textContent='Avant arrosage : '+(s.config.dryRaw||'non mesuré')+' · Après arrosage : '+(s.config.wetRaw||'non mesuré');
 showAdvice(s);populateSettings(s);
}
async function loadStatus(){try{render(await fetchJson('/api/status'));}catch(e){$('connection').textContent='Station inaccessible';$('dot').classList.add('off');$('fresh').textContent='Hors ligne';}}
function mergePoints(a,b){const all=[...a,...b].filter(p=>p&&p.epoch>1700000000).sort((x,y)=>x.epoch-y.epoch);const seen=new Map();for(const p of all)seen.set(p.epoch,p);return [...seen.values()];}
async function loadHistory(){if(busyHistory)return;busyHistory=true;try{
 const h=hours;const [past,newLive]=await Promise.all([fetchJson('/api/history?hours='+h),fetchJson('/api/live')]);
 if(hours!==h)return;live=newLive;
 history=mergePoints(past,newLive);drawCharts();analyzeLight(history);analyzeHumidity(history);
 }catch(e){toast('Historique : '+e.message);}finally{busyHistory=false;}}
async function loadWater(){try{
  const response=await fetch('/arrosages.csv',{cache:'no-store'});if(!response.ok)throw Error('Journal inaccessible');
  const content=await response.text();waterTimes=content.split(/\r?\n/).map(s=>Number(s.trim())).filter(t=>t>1700000000).sort((a,b)=>b-a);const list=waterTimes.slice(0,12);if(tab==='graphs')drawCharts();
  const target=$('waterLog');target.replaceChildren();
  if(!list.length){target.textContent='Aucun arrosage enregistré pour le moment.';return;}
  for(const t of list){const item=document.createElement('div');item.className='event';const a=document.createElement('b');a.textContent='Arrosage';const b=document.createElement('span');b.textContent=stamp(t);item.append(a,b);target.append(item);}
 }catch(e){$('waterLog').textContent=e.message;}}
const chartNames={soil:'ADC',temperature:'°C',humidity:'% HR',lux:'lux',pressure:'hPa',soilTemperature:'°C',compareTemp:'°C'};
const chartColors={soil:'#2e8b62',temperature:'#d9926d',humidity:'#619bc0',lux:'#d7a348',pressure:'#9a8cbd',soilTemperature:'#7dba91',compareTemp:'#d9926d'};
function setExportLink(){const a=$('exportPeriod'),labels={1:'1 h',6:'6 h',24:'24 h',168:'7 jours',720:'30 jours'};a.href='/export.csv?hours='+hours;a.textContent='Exporter '+labels[hours]+' (CSV complet)';}
function chartDomain(){
 if(!history.length)return null;
 const t0=history[0].epoch,t1=history[history.length-1].epoch;
 const entire=Math.max(60,t1-t0),span=Math.max(120,entire/graphZoom);
 const back=Math.max(0,entire-span)*graphPan;
 return {start:t1-back-span,end:t1-back,span};
}
function chartPoints(key){return history.filter(p=>p.epoch>1700000000&&p[key]!=null&&Number.isFinite(Number(p[key]))).map(p=>({t:p.epoch,v:Number(p[key])}));}
function nearest(pts,t){if(!pts.length)return null;let lo=0,hi=pts.length-1;while(lo<hi){const m=(lo+hi)>>1;if(pts[m].t<t)lo=m+1;else hi=m;}if(lo>0&&Math.abs(pts[lo-1].t-t)<Math.abs(pts[lo].t-t))lo--;return pts[lo];}
function drawOne(canvas){
 const key=canvas.dataset.series,ctx=canvas.getContext('2d'),rect=canvas.getBoundingClientRect(),w=Math.max(250,Math.floor(rect.width)),h=Math.max(120,Math.floor(rect.height)),dpr=Math.min(2,window.devicePixelRatio||1);
 canvas.width=w*dpr;canvas.height=h*dpr;ctx.setTransform(dpr,0,0,dpr,0,0);ctx.clearRect(0,0,w,h);
 const dark=matchMedia('(prefers-color-scheme: dark)').matches,dom=chartDomain(),detail=$('detail-'+key);
 ctx.font='11px ui-sans-serif, sans-serif';
 if(!dom){detail.textContent='Aucun relevé enregistré.';return;}
 const left=49,right=12,top=16,bottom=29,x0=left,x1=w-right,y0=top,y1=h-bottom;
 const names=key==='compareTemp'?['temperature','soilTemperature']:[key];
 const lines=names.map(k=>chartPoints(k).filter(p=>p.t>=dom.start&&p.t<=dom.end));
 const all=lines.flat();
 if(all.length<2){ctx.textAlign='center';ctx.fillStyle=dark?'#aec1af':'#899b8d';ctx.fillText('Pas assez de données sur cette période',w/2,h/2);detail.textContent='Élargis la période ou attends de nouvelles mesures.';return;}
 let mi=Math.min(...all.map(p=>p.v)),ma=Math.max(...all.map(p=>p.v)),margin=(ma-mi)*.12||Math.max(.5,Math.abs(ma)*.025);
 mi-=margin;ma+=margin;
 const x=t=>x0+(t-dom.start)/Math.max(1,dom.span)*(x1-x0),y=v=>y1-(v-mi)/Math.max(.00001,ma-mi)*(y1-y0);
 ctx.strokeStyle=dark?'#3d5947':'#e8ede7';ctx.lineWidth=1;ctx.fillStyle=dark?'#acc0ac':'#75867a';ctx.textAlign='right';
 for(let i=0;i<=3;i++){const yy=y0+(y1-y0)*i/3;ctx.beginPath();ctx.moveTo(x0,yy);ctx.lineTo(x1,yy);ctx.stroke();ctx.fillText((ma-(ma-mi)*i/3).toFixed(key==='soil'||key==='lux'?0:1),x0-7,yy+4);}
 ctx.textAlign='center';const axisFmt=t=>new Date(t*1000).toLocaleString('fr-BE',{day:'2-digit',month:'2-digit',hour:'2-digit',minute:'2-digit'});ctx.fillText(axisFmt(dom.start),x0+30,h-5);ctx.fillText(axisFmt(dom.end),w-52,h-5);
 if(key==='soil')for(const t of waterTimes){if(t<dom.start||t>dom.end)continue;ctx.setLineDash([4,4]);ctx.strokeStyle='#429ad3';ctx.beginPath();ctx.moveTo(x(t),y0);ctx.lineTo(x(t),y1);ctx.stroke();ctx.setLineDash([]);ctx.fillStyle='#429ad3';ctx.textAlign='center';ctx.fillText('arrosage',Math.max(82,Math.min(w-50,x(t))),13);}
 let gapCount=0;
 lines.forEach((pts,j)=>{
   ctx.strokeStyle=chartColors[names[j]];ctx.lineWidth=2.2;ctx.lineJoin='round';ctx.beginPath();
   for(let i=0;i<pts.length;i++){
     const p=pts[i],wasGap=i>0&&(p.t-pts[i-1].t>900);if(wasGap)gapCount++;
     if(i===0||wasGap)ctx.moveTo(x(p.t),y(p.v));else ctx.lineTo(x(p.t),y(p.v));
   }ctx.stroke();
   const p=pts[pts.length-1];if(p){ctx.fillStyle=chartColors[names[j]];ctx.beginPath();ctx.arc(x(p.t),y(p.v),3,0,Math.PI*2);ctx.fill();}
 });
 const vals=all.map(p=>p.v);$('stat-'+key).textContent='Min '+val(Math.min(...vals),chartNames[key],1)+' · Max '+val(Math.max(...vals),chartNames[key],1);
 if(graphCursor!=null&&graphCursor>=dom.start&&graphCursor<=dom.end){
   ctx.save();ctx.setLineDash([3,4]);ctx.strokeStyle=dark?'#deded3':'#506959';ctx.lineWidth=1;ctx.beginPath();ctx.moveTo(x(graphCursor),y0);ctx.lineTo(x(graphCursor),y1);ctx.stroke();ctx.restore();
   let txt=[];
   for(let j=0;j<lines.length;j++){
     const p=nearest(lines[j],graphCursor);if(p&&Math.abs(p.t-graphCursor)<=900){ctx.fillStyle=chartColors[names[j]];ctx.beginPath();ctx.arc(x(p.t),y(p.v),4.5,0,Math.PI*2);ctx.fill();txt.push((key==='compareTemp'?(j===0?'Air':'Racines'):'Valeur')+' : '+val(p.v,chartNames[key],key==='soil'?0:2));}
   }
   detail.textContent=new Date(graphCursor*1000).toLocaleString('fr-BE',{dateStyle:'short',timeStyle:'medium'})+' · '+(txt.length?txt.join(' · '):'Pas de mesure à cette heure');
 }else detail.textContent='Touchez ou faites glisser sur la courbe pour afficher la mesure exacte et son heure.';
 if(gapCount)detail.textContent+=' · '+gapCount+' interruption(s) de mesures dans cette vue.';
}
function drawCharts(){document.querySelectorAll('canvas[data-series]').forEach(drawOne);const d=chartDomain();if(d)$('graphRange').textContent=stamp(d.start)+' → '+stamp(d.end);$('graphEarlier').disabled=graphPan>=1;$('graphLater').disabled=graphPan<=0;$('graphZoomOut').disabled=graphZoom<=1;}
function onGraphPointer(ev){
 const canvas=ev.currentTarget,dom=chartDomain();if(!dom)return;
 const rect=canvas.getBoundingClientRect(),ratio=Math.max(0,Math.min(1,(ev.clientX-rect.left-49)/Math.max(1,rect.width-61)));
 const guess=dom.start+ratio*dom.span;
 const k=canvas.dataset.series,pts=chartPoints(k==='compareTemp'?'temperature':k).filter(p=>p.t>=dom.start&&p.t<=dom.end);
 const p=nearest(pts,guess);if(!p)return;graphCursor=p.t;drawCharts();
}
document.querySelectorAll('canvas[data-series]').forEach(c=>{
 c.addEventListener('pointermove',e=>{if(e.pointerType==='mouse'||e.buttons)onGraphPointer(e)});
 c.addEventListener('pointerdown',onGraphPointer);
 c.addEventListener('keydown',e=>{if(e.key==='ArrowLeft'||e.key==='ArrowRight'){
  e.preventDefault();const dom=chartDomain(),pts=chartPoints(c.dataset.series==='compareTemp'?'temperature':c.dataset.series).filter(p=>p.t>=dom.start&&p.t<=dom.end);if(!pts.length)return;
  const current=nearest(pts,graphCursor??pts[pts.length-1].t),i=pts.indexOf(current),next=Math.max(0,Math.min(pts.length-1,i+(e.key==='ArrowLeft'?-1:1)));graphCursor=pts[next].t;drawCharts();
 }});
 c.tabIndex=0;
});
$('graphZoomIn').onclick=()=>{const d=chartDomain();graphZoom=Math.min(16,graphZoom*2);graphPan=0;graphCursor=null;drawCharts();};
$('graphZoomOut').onclick=()=>{graphZoom=Math.max(1,graphZoom/2);graphPan=0;graphCursor=null;drawCharts();};
$('graphEarlier').onclick=()=>{graphPan=Math.min(1,graphPan+.25);graphCursor=null;drawCharts();};
$('graphLater').onclick=()=>{graphPan=Math.max(0,graphPan-.25);graphCursor=null;drawCharts();};
$('graphReset').onclick=()=>{graphZoom=1;graphPan=0;graphCursor=null;drawCharts();};
setExportLink();
function analyzeHumidity(points){
 const el=$('humidityAdvice'),valid=points.filter(p=>p.humidity!=null&&Number.isFinite(Number(p.humidity))&&p.epoch>1700000000).sort((a,b)=>a.epoch-b.epoch);
 if(!valid.length){el.textContent='Humidité : en attente de mesures fiables près des feuilles.';return;}
 const last=valid[valid.length-1],recent=valid.filter(p=>p.epoch>=last.epoch-2*3600);
 const covered=recent.length>=12&&recent[recent.length-1].epoch-recent[0].epoch>90*60&&!recent.slice(1).some((p,i)=>p.epoch-recent[i].epoch>900);
 const low=covered&&recent.filter(p=>p.humidity<35).length/recent.length>=.8;
 if(low)el.textContent='Air sec persistant pendant environ 2 h. Vérifie que l’AHT20 est éloigné de l’ESP32 ; envisage un petit humidificateur de pièce si besoin. Évite de brumiser systématiquement le feuillage : effet bref et humidité résiduelle sur les feuilles.';
 else if(last.humidity<35)el.textContent='Air sec sur la dernière mesure, mais durée encore incertaine. Vérifie le placement du capteur avant toute action.';
 else el.textContent='Aucune brumisation nécessaire : surveille la tendance de l’humidité ambiante, en gardant une bonne ventilation.';
}
function analyzeLight(points){
 const byDay=new Map();const sorted=points.filter(p=>p.lux!=null&&Number.isFinite(Number(p.lux))).sort((a,b)=>a.epoch-b.epoch);
 for(let i=0;i<sorted.length;i++){
  const p=sorted[i],d=new Date(p.epoch*1000),hour=d.getHours();if(hour<8||hour>=20)continue;
  const day=d.toLocaleDateString('sv-SE'),info=byDay.get(day)||{max:0,luxh:0,first:p.epoch,last:p.epoch,n:0};
  const dt=i?Math.min(300,p.epoch-sorted[i-1].epoch):0;
  info.max=Math.max(info.max,Number(p.lux));info.luxh+=Number(p.lux)*Math.max(0,dt)/3600;info.last=p.epoch;info.n++;byDay.set(day,info);
 }
 const days=[...byDay.entries()].filter(([,v])=>v.last-v.first>=6*3600 && v.n>=20).sort((a,b)=>a[0].localeCompare(b[0]));
 if(!days.length){$('lightAdvice').textContent='Il manque une journée assez complète à la bonne position du BH1750. Évite de modifier la lampe sur une mesure isolée.';$('lightSum').textContent='Analyse en attente · journées suffisantes : 0';return;}
 const [day,last]=days[days.length-1];$('lightSum').textContent=day+' · maximum '+val(last.max,'lux',0)+' · exposition estimée '+val(last.luxh,'lux·h',0);
 if(last.max<2000){$('lightAdvice').textContent='Luminosité mesurée faible sur une journée couverte. Vérifie d’abord le placement du BH1750, puis la distance et l’orientation de ta lampe.';}
 else if(days.length>=3&&days.slice(-3).every(([,v])=>v.max<3000)){$('lightAdvice').textContent='Trois journées sous le repère indicatif de 3 000 lux. Vérifie la lampe et le placement de la station.';}
 else $('lightAdvice').textContent='De la lumière a été détectée sur une journée. Observe plusieurs journées et les feuilles avant d’ajuster la minuterie.';
}
$('waterBtn').onclick=async()=>{if(await action('/api/water',{},'As-tu réellement arrosé DragonFruit et laissé l’eau s’écouler ?'))loadWater();};
$('calStartBtn').onclick=()=>action('/api/calibration/start',{},'Lancer 16 relevés sur 5 minutes sans toucher à la sonde ?');
$('movedBtn').onclick=()=>action('/api/calibration/moved',{},'Confirmer le déplacement ? Les références avant/après arrosage seront réinitialisées, mais les données conservées.');
$('beforeBtn').onclick=()=>action('/api/calibration/reference',{kind:'before'},'Confirmer que le substrat a réellement besoin d’un arrosage maintenant ?');
$('afterBtn').onclick=()=>action('/api/calibration/reference',{kind:'after'},'Confirmer que l’arrosage est terminé et que le pot s’est égoutté 30–60 min ?');
$('telegramTestBtn').onclick=()=>action('/api/telegram/test');
$('settingsForm').oninput=()=>{settingsDirty=true;};
$('settingsForm').onsubmit=async e=>{e.preventDefault();const f=e.target;const data={};for(const k of ['tLow','tHigh','rhLow','rhHigh','lightLow'])data[k]=f.elements[k].value;data.telegram=f.elements.telegram.checked?'1':'0';data.lightAlerts='0';if(await action('/api/settings',data))settingsDirty=false;};
const periodic=async()=>{await loadStatus();if((tab==='graphs'||tab==='home') && Date.now()-periodic.graphTime>60000){periodic.graphTime=Date.now();loadHistory();}};
periodic.graphTime=0;
window.addEventListener('resize',()=>{if(tab==='graphs')drawCharts();});
loadStatus();loadHistory();setInterval(periodic,4000);
</script>
</body></html>
)DRAGON";
