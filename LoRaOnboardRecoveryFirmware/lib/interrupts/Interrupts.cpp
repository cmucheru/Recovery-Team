#include <Interrupts.h>
#include <Ejection.h>

const uint8_t DROGUE_SEPARATION_BREAK_WIRE_PIN = 12;
const uint8_t MAIN_SEPARATION_BREAK_WIRE_PIN = 13;
const uint8_t LAUNCH_BREAK_WIRE_PIN = 25;
const uint8_t PRIMARY_DROGUE_DETECT_PIN = 36;
const uint8_t PRIMARY_MAIN_DETECT_PIN = 37;

TimerHandle_t motorTimerHandle = NULL;
extern uint8_t MAIN_EJECTION_PIN;
extern TaskHandle_t OnReceiveTaskHandle;

volatile bool isDrogueSeparated = false;
volatile bool isMainSeparated = false;
volatile bool isLaunched = false;
volatile bool isPrimaryDrogueFired = false;
volatile bool isPrimaryMainFired = false;

// TODO: adjust time to simulation apogee time for ejection
int MOTOR_EJECT_TIME = 3000;

void setInterruptPins()
{
    pinMode(DROGUE_SEPARATION_BREAK_WIRE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(DROGUE_SEPARATION_BREAK_WIRE_PIN), setDrogueSeparated, RISING);

    pinMode(MAIN_SEPARATION_BREAK_WIRE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(MAIN_SEPARATION_BREAK_WIRE_PIN), setMainSeparated, RISING);

    pinMode(LAUNCH_BREAK_WIRE_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(LAUNCH_BREAK_WIRE_PIN), startMotorEjectionCounter, RISING);

    pinMode(PRIMARY_DROGUE_DETECT_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PRIMARY_DROGUE_DETECT_PIN), detectPrimaryDrogueEjected, RISING);

    pinMode(PRIMARY_MAIN_DETECT_PIN, INPUT_PULLDOWN);
    attachInterrupt(digitalPinToInterrupt(PRIMARY_MAIN_DETECT_PIN), detectPrimaryMainEjected, RISING);
}

void MotorEjectTimerCallback(TimerHandle_t ejectionTimerHandle)
{
    ejection(MAIN_EJECTION_PIN);
}
void IRAM_ATTR startMotorEjectionCounter()
{
    isLaunched = true;
    motorTimerHandle = xTimerCreate("MotorEjectionTimer", MOTOR_EJECT_TIME / portTICK_PERIOD_MS, pdFALSE, (void *)0, MotorEjectTimerCallback);
    xTimerStart(motorTimerHandle, portMAX_DELAY);
    detachInterrupt(digitalPinToInterrupt(LAUNCH_BREAK_WIRE_PIN));
}
void IRAM_ATTR setDrogueSeparated()
{
    isDrogueSeparated = true;
    detachInterrupt(digitalPinToInterrupt(DROGUE_SEPARATION_BREAK_WIRE_PIN));
}
void IRAM_ATTR setMainSeparated()
{
    isMainSeparated = true;
    detachInterrupt(digitalPinToInterrupt(MAIN_SEPARATION_BREAK_WIRE_PIN));
}

void IRAM_ATTR detectPrimaryMainEjected()
{
    isPrimaryMainFired = true;
}
void IRAM_ATTR detectPrimaryDrogueEjected()
{
    isPrimaryDrogueFired = true;
}
