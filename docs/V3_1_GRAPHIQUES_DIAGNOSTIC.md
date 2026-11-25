# DragonFruit Monitor V3.1 — Graphiques interactifs et diagnostics

**Prototype à tester sur ton ESP32.** Cette version part du firmware V3 de `ADMDM/dragonfruit-monitor` et conserve le câblage GPIO34, GPIO21/22 et GPIO27 et les fichiers CSV `/d<jour>.csv`, `/v3/d<jour>.csv`.

## Nouveautés

- Graphiques interactifs : tap sur iPhone, survol et clic sur Mac pour connaître la valeur exacte et son horodatage (avec secondes) ; commandes de zoom, plus tôt, plus récent et réinitialisation ; export CSV sur 1 h, 6 h, 24 h, 7 j ou 30 j.
- Courbes séparées en cas de plus de 15 minutes sans mesure, avec indication du nombre d'interruptions dans la vue.
- Repères des arrosages enregistrés sur la courbe du substrat.
- Nouveau graphique comparant température de l'air (AHT20) et température des racines (DS18B20).
- Analyse de l'air sec persistant et recommandations prudentes. **Ne pas utiliser la brumisation comme remède systématique** ; elle humidifie peu et brièvement l'air. Contrôler la position de l'AHT20 puis, si nécessaire, envisager un humidificateur ambiant, avec ventilation.
- Sur l'ESP32, état de l'écriture LittleFS, quantité de RAM libre et code de dernière cause de redémarrage.
- Notification Telegram en cas d'échec de sauvegarde **si le Wi-Fi et Telegram fonctionnent encore**, puis notification lors du rétablissement. Message après une coupure Wi-Fi longue **seulement au retour de la connexion**.
- Intégration facultative Healthchecks.io pour signaler une panne totale ou l'absence d'une sauvegarde récente par un service extérieur.

## Installation sans perdre l'historique

1. Ouvrir l'application actuelle et sauvegarder **`/export.csv?hours=720`** et **`/arrosages.csv`** sur le Mac.
2. Décompresser le projet, ouvrir `DragonFruit_V3.ino` dans Arduino IDE en gardant tous les fichiers `.cpp` et `.h` dans le même dossier.
3. **Copier son `secrets.h` existant** dans le nouveau dossier (ne jamais le publier). Sinon copier `secrets.example.h` vers `secrets.h` et renseigner les identifiants localement.
4. Garder exactement la même carte ESP32, le même **Partition Scheme**, le même port et les mêmes bibliothèques que pour V3.0. Ne pas choisir **Erase All Flash** ni formater LittleFS.
5. Cliquer sur **Vérifier** avant téléversement. Le résultat doit tenir dans la partition de l'application (V3.0 utilisait déjà environ 96 %). Si la compilation dépasse la limite, **ne change pas le schéma de partition** pour contourner le problème ; demande plutôt une optimisation du code.
6. Téléverser seulement après compilation réussie. L'acquisition s'interrompt pendant le transfert ; elle reprend ensuite.
7. Tester `http://dragonfruit.local` (ou l'adresse IP), onglet **Graphiques**. Vérifier l'horodatage des points, le zoom, l'export CSV 24 h, les repères d'arrosage. Vérifier une sauvegarde après cinq minutes.

## Surveillance d'une station hors ligne (facultatif)

La carte **ne peut pas envoyer de notification si elle est sans alimentation ou déconnectée du réseau**. Seul un système externe sait constater l'absence d'un signal.

1. Créer un compte sur https://healthchecks.io et un contrôle `DragonFruit Monitor`, **Period 5 minutes**, **Grace Time 5 minutes**. Activer l'intégration Telegram ou e-mail.
2. Dans le fichier **local** `secrets.h`, définir :

   ```cpp
   #define DRAGON_HEALTHCHECKS_URL "https://hc-ping.com/TON_UUID_PRIVE"
   #define DRAGON_HEALTHCHECKS_ACTIVE 1
   ```

   Garder ces deux paramètres **secrets**. Si les mêmes macros figurent dans `secrets.example.h` et que tu copies cet exemple, modifie les lignes existantes plutôt que d'en ajouter des doublons.
3. Recompiler et téléverser. Si la V3.1 peut sauvegarder dans LittleFS et joindre Internet, elle envoie un signal après chaque sauvegarde de cinq minutes. Healthchecks déclenche une notification si aucun signal n'arrive dans les délais définis. Tester la fonction avec une **coupure contrôlée** lorsque tu peux surveiller la carte.

**Sécurité TLS :** pour rester compatible avec la partition programme presque remplie, le module facultatif utilise actuellement une connexion HTTPS dont le certificat du serveur **n'est pas vérifié** (`setInsecure`). C'est un prototype : la confidentialité et l'authenticité de l'URL de ping ne sont donc pas garanties contre un attaquant sur le réseau. Pour un fonctionnement durable, il faudra ajouter une racine CA appropriée et revalider la taille du firmware. Cette fonction est **désactivée par défaut** et ne doit pas être considérée comme active tant que le ping et l'alerte externe n'ont pas été testés.

## Calibration de l'arrosage

Les premières références observées après l'arrosage réel du 25 novembre 2026 vers 00 h sont de l'ordre de **1488 avant** et **736 après**, à position de sonde inchangée. Ce sont des repères expérimentaux **non validés comme seuils de nécessité d'arrosage**. Dans l'interface, saisir la référence « avant » uniquement si l'état du substrat au toucher confirmait le besoin d'arroser, puis la référence « après » quand le pot s'est égoutté. Après plusieurs cycles, l'application estimera quand **vérifier** le substrat. Elle ne doit jamais déclencher un arrosage automatique sur cette seule base.

## Compatibilité

- ESP32-WROOM-32 Arduino, AHT20+BMP280, BH1750, DS18B20 et sonde capacitive analogique.
- La balance HX711 reste une évolution **non implémentée** dans cette version.
- Front-end sans dépendance CDN externe ; les graphiques n'utilisent pas Chart.js.
- Le code JavaScript a passé la vérification syntaxique Node.js et des tests fonctionnels simulés de rendu des graphes. Le firmware C++ modifié **n'a pas encore été compilé avec la configuration exacte de ton Arduino IDE ni testé sur la carte** : cela reste indispensable avant installation.
