# Sécurité

Ne pas ajouter `secrets.h` à GitHub. Utiliser `secrets.example.h` comme modèle.

- Configurer un token Telegram privé et un code de commande local unique.
- Ne pas exposer le port HTTP de l'ESP32 directement sur Internet ; les commandes locales sont en HTTP non chiffré.
- Révoquer immédiatement dans BotFather tout token accidentellement partagé.
- Sauvegarder les exports CSV sur un stockage privé. GitHub ne remplace pas le stockage de l'ESP32.
- Vérifier que le dépôt GitHub est **privé** avant toute publication de données supplémentaires.
