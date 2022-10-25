#include <Ejection.h>

TimerHandle_t ejectionTimerHandle = NULL;

// Pin to start ejection charge
uint8_t MAIN_EJECTION_PIN = 23;
uint8_t DROGUE_EJECTION_PIN = 22;

volatile bool isManualMainFired = false;
volatile bool isManualDrogueFired = false;

// TODO: carry out tests to know how much time
const int EJECTION_PIN_ON_TIME_MS = 1000;

char DROGUE_MESSAGE[] = "D";
char MAIN_MESSAGE[] = "M";

void setEjectionPinModes()
{
    pinMode(DROGUE_EJECTION_PIN, OUTPUT);
    pinMode(MAIN_EJECTION_PIN, OUTPUT);
}

//  switch off drogue pyro-channel
void DrogueEjectionTimerCallback(TimerHandle_t ejectionTimerHandle)
{
    digitalWrite(DROGUE_EJECTION_PIN, LOW);
}

// switch off main pyro-channel
void MainEjectionTimerCallback(TimerHandle_t ejectionTimerHandle)
{
    digitalWrite(MAIN_EJECTION_PIN, LOW);
}

// Ejection fires the explosive charge using a mosfet
void ejection(uint8_t ejectionPin)
{
    if (ejectionPin == MAIN_EJECTION_PIN)
    {
        digitalWrite(MAIN_EJECTION_PIN, HIGH);
        isManualMainFired = true;
        ejectionTimerHandle = xTimerCreate("MainEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, MainEjectionTimerCallback);
        xTimerStart(ejectionTimerHandle, portMAX_DELAY);
    }
    else if (ejectionPin == DROGUE_EJECTION_PIN)
    {
        digitalWrite(DROGUE_EJECTION_PIN, HIGH);
        isManualDrogueFired = true;
        ejectionTimerHandle = xTimerCreate("DrogueEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, DrogueEjectionTimerCallback);
        xTimerStart(ejectionTimerHandle, portMAX_DELAY);
    }
}