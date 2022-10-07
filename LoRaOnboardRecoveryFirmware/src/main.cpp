
#include "heltec.h"
#include "../include/defs.h"
#include "../include/initlora.h"
#include "../include/initsensors.h"

TaskHandle_t GPSTaskHandle = NULL;
TimerHandle_t ejectionTimerHandle = NULL;

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
    ejectionTimerHandle = xTimerCreate("MainEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, MainEjectionTimerCallback);
    xTimerStart(ejectionTimerHandle, portMAX_DELAY);
  }
  else if (ejectionPin == DROGUE_EJECTION_PIN)
  {
    digitalWrite(DROGUE_EJECTION_PIN, HIGH);
    ejectionTimerHandle = xTimerCreate("DrogueEjectionTimer", EJECTION_PIN_ON_TIME_MS / portTICK_PERIOD_MS, pdFALSE, (void *)0, DrogueEjectionTimerCallback);
    xTimerStart(ejectionTimerHandle, portMAX_DELAY);
  }
}

void readGPSTask(void *parameter)
{

  struct GPSReadings gpsReadings = {0};

  for (;;)
  {
    gpsReadings = get_gps_readings();

    debugf("Latitude : %.3f\n", gpsReadings.latitude);
    debugf("Longitude : %.3f\n", gpsReadings.longitude);

    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void onReceive(int packetSize)
{
  char command[2];
  for (int i = 0; i < packetSize; i++)
  {
    command[i] = (char)LoRa.read();
  }
  if (command == DROGUE_MESSAGE)
  {
    ejection(DROGUE_EJECTION_PIN);
  }
  else if (command == MAIN_MESSAGE)
  {
    ejection(MAIN_EJECTION_PIN);
  }
}

void setPinModes()
{
  pinMode(DROGUE_EJECTION_PIN, OUTPUT);
  pinMode(MAIN_EJECTION_PIN, OUTPUT);
}
void setup()
{
  initHeltecLoRa();

  // register the receive callback
  LoRa.onReceive(onReceive);

  // Set output pin modes
  setPinModes();

  //init_gps();

  // Create tasks on core 1
  //xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 3000, NULL, 1, &GPSTaskHandle, app_cpu);

  vTaskDelete(NULL);
}

void loop()
{
}