#ifndef EJECTION_H
#define EJECTION_H

#include "../definitions/defs.h"

void DrogueEjectionTimerCallback(TimerHandle_t ejectionTimerHandle);
void MainEjectionTimerCallback(TimerHandle_t ejectionTimerHandle);
void ejection(uint8_t ejectionPin);
void setEjectionPinModes();

#endif // EJECTION_H