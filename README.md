# DragonFruit Monitor V3

Station connectée ESP32 pour **DragonFruit — Faux Poivrier** : suivi du substrat, de la température, de l'humidité de l'air, de la pression et de la lumière. Interface mobile et alertes Telegram.

> **Statut : prototype V3.0.** Une compilation Arduino a été réalisée par le propriétaire. Le projet reste à valider sur une période prolongée, notamment la stabilité du sol, les historiques et les seuils. Les conseils d'arrosage sont provisoires tant que les références avant/après arrosage ne sont pas vérifiées.

## Matériel et branchements

| Module | Connexion ESP32 | Rôle |
| --- | --- | --- |
| Sonde capacitive analogique | GPIO34, 3,3 V, GND | Indice relatif du substrat |
| AHT20 + BMP280 | I²C SDA GPIO21, SCL GPIO22, 3,3 V | Température, HR, pression |
| BH1750 | I²C SDA GPIO21, SCL GPIO22, 3,3 V | Éclairement lux |
| DS18B20 (facultatif) | DATA GPIO27, 3,3 V, GND, pull-up 4,7 kΩ | Température du substrat |

**Sécurité :** débrancher l'USB avant tout câblage ; garder toutes les connexions à l'abri de l'arrosage ; ne jamais envoyer 5 V dans un GPIO ESP32. Le DS18B20 est optionnel et ne doit pas bloquer les mesures des autres capteurs.

## Installer sur Arduino IDE

1. Dans `firmware/DragonFruit_V3`, **copier** `secrets.example.h` sous le nom `secrets.h` ; renseigner localement les identifiants Wi-Fi, Telegram et un code de contrôle local fort.
2. Ouvrir `DragonFruit_V3.ino` avec Arduino IDE et sélectionner **ESP32 Dev Module**.
3. Installer `BH1750`, `Adafruit AHTX0`, `Adafruit BMP280 Library`, `OneWire`, `DallasTemperature`, `UniversalTelegramBot` et `ArduinoJson`. Le cœur Arduino-ESP32 fournit WiFi, WebServer, LittleFS, Preferences, Wire et ESPmDNS.
4. **Avant le flashage**, exporter `/export.csv` et `/arrosages.csv` de la station existante ; **conserver exactement le même schéma de partitionnement** (LittleFS) et ne pas choisir « Erase All Flash ».
5. Vérifier, puis téléverser. Ouvrir le Moniteur série à 115200 bauds ; consulter l'adresse IP affichée ou `http://dragonfruit.local` si le réseau prend en charge mDNS.
6. Vérifier que les nouvelles mesures sont sauvegardées toutes les cinq minutes, que l'export contient `origine=V3`, et que le test Telegram fonctionne.

Guide détaillé : [docs/INSTALLATION.md](docs/INSTALLATION.md).

## Ce que la V3 mesure et enregistre

- Échantillonnage toutes les 5 secondes ; sauvegarde des valeurs toutes les 5 minutes dans LittleFS, après synchronisation de l'horloge.
- Historique V2.2 conservé dans les anciens fichiers `/d...csv` ; nouveaux relevés dans `/v3/d...csv`.
- Interface locale avec graphiques, journal d'arrosages, calibration de stabilité de 5 minutes et réglages d'alertes.
- Notifications Telegram et commandes de consultation. Le bot ne contrôle pas physiquement la lampe.
- DS18B20 déjà pris en charge par le code, **sans matériel supplémentaire nécessaire pour compiler** ; absent tant qu'il n'est pas raccordé.

## Confidentialité et sauvegardes

`secrets.h`, les CSV de mesures, les tokens et mots de passe ne sont **jamais versionnés**. Il ne faut pas les déposer sur GitHub via l'interface Web, car un simple glisser-déposer ne tient pas compte du `.gitignore`. Seul `secrets.example.h` (sans données privées) appartient au dépôt.

La mémoire LittleFS de l'ESP32 **n'est pas sauvegardée par GitHub**. Exporter régulièrement les mesures en lieu sûr. La sauvegarde automatique cloud est une amélioration prévue et n'est pas encore opérationnelle.

## Améliorations prévues

- Détection de valeurs aberrantes et confirmation des références de calibration sur plusieurs cycles.
- Alertes lumière basées sur plusieurs journées et emplacement du BH1750.
- Sauvegarde externe automatique et restauration guidée.
- Réduction de la taille du binaire (environ 96 % de l'espace de programme dans une compilation signalée).
- Tests de compatibilité des bibliothèques Arduino et documentation de validation sur ESP32.

Ce dépôt contient le **code**, pas les données historiques personnelles. Aucune licence de réutilisation n'a été définie à ce stade.
