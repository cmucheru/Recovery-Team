#ifndef INTERRUPTS_H
#define INTERRUPTS_H

#include "../definitions/defs.h"

void setInterruptPins();
void MotorEjectTimerCallback(TimerHandle_t ejectionTimerHandle);
void IRAM_ATTR startMotorEjectionCounter();
void IRAM_ATTR setDrogueSeparated();
void IRAM_ATTR setMainSeparated();
void IRAM_ATTR detectPrimaryMainEjected();
void IRAM_ATTR detectPrimaryDrogueEjected();

#endif // INTERRUPTS_H