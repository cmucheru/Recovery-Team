
#include "heltec.h"
#include "../include/defs.h"
#include "../include/initlora.h"
#include "../include/initsensors.h"

TaskHandle_t GPSTaskHandle = NULL;
TaskHandle_t SendLoRaTaskHandle = NULL;
TimerHandle_t ejectionTimerHandle = NULL;

static uint16_t gps_queue_length = 100;
static QueueHandle_t gps_queue;

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
  static int droppedGPSPackets = 0;

  for (;;)
  {
    gpsReadings = get_gps_readings();

    if (xQueueSend(gps_queue, (void *)&gpsReadings, 0) != pdTRUE)
    {
      droppedGPSPackets++;
    }

    debugf("Dropped GPS Packets : %d\n", droppedGPSPackets);

    // yield to idle task
    vTaskDelay(10 / portTICK_PERIOD_MS);
  }
}
void sendLoRaTask(void *parameter)
{
  struct SendValues sv = {0};
    struct GPSReadings gpsReadings = {0};
    float latitude = 0;
    float longitude = 0;

  for (;;)
  {
    sv.latitude = latitude;
    sv.longitude = longitude;

    if (xQueueReceive(gps_queue, (void *)&gpsReadings, 10) == pdTRUE)
    {
      //TODO: start here
      latitude = gpsReadings.latitude;
      longitude = gpsReadings.longitude;
    }
    sendTelemetryLora(sv);
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
  if (strcmp(command, DROGUE_MESSAGE) == 0)
  {
    ejection(DROGUE_EJECTION_PIN);
  }
  else if (strcmp(command, MAIN_MESSAGE) == 0)
  {
    ejection(MAIN_EJECTION_PIN);
  }
  debugln(LoRa.packetRssi());
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
  // LoRa.onReceive(onReceive);
  // LoRa.receive();

  // Set output pin modes
  setPinModes();

  init_gps();

  // Create tasks on core 1
  xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 3000, NULL, 1, &GPSTaskHandle, app_cpu);

  // Create tasks on core 0
  xTaskCreatePinnedToCore(sendLoRaTask, "LoRaGPSTask", 3000, NULL, 1, &SendLoRaTaskHandle, pro_cpu);

  vTaskDelete(NULL);
}

void loop()
{
}