#ifndef DEFS_H
#define DEFS_H
#include "heltec.h"

#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif // DEBUG

extern volatile bool isDrogueSeparated;
extern volatile bool isMainSeparated;
extern volatile bool isLaunched;
extern volatile bool isPrimaryDrogueFired;
extern volatile bool isPrimaryMainFired;
extern volatile bool isManualMainFired;
extern volatile bool isManualDrogueFired;

#endif // DEFS_H