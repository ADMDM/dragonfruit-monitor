// DragonFruit V3 - le sketch reste volontairement minimal.
// Toute la logique est dans dragon_core.cpp pour eviter les prototypes
// automatiques de l'IDE Arduino et l'erreur "Alerte non declare" de V2.2.
#include "dragon_core.h"
void setup() { dragonSetup(); }
void loop()  { dragonLoop();  }
