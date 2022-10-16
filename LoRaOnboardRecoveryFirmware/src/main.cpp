#include "../lib/definitions/defs.h"
#include <Sensors.h>
#include <Ejection.h>
#include <NKJLoRa.h>
#include <Tasks.h>

TaskHandle_t GPSTaskHandle = NULL;
TaskHandle_t SendLoRaTaskHandle = NULL;

const BaseType_t pro_cpu = 0;
const BaseType_t app_cpu = 1;

QueueHandle_t gps_queue;

const uint16_t GPS_QUEUE_LENGTH = 100;

void setup()
{
  initHeltecLoRa();

  setEjectionPinModes();

  // register the receive callback
  LoRa.onReceive(onReceive);
  LoRa.receive();


  init_gps();

  // create GPS queue
  gps_queue = xQueueCreate(GPS_QUEUE_LENGTH, sizeof(GPSReadings));

  // Create tasks on core 1
  xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 3000, NULL, 1, &GPSTaskHandle, app_cpu);

  // Create tasks on core 0
  xTaskCreatePinnedToCore(sendLoRaTask, "LoRaGPSTask", 3000, NULL, 1, &SendLoRaTaskHandle, pro_cpu);

  vTaskDelete(NULL);
}

void loop()
{
  vTaskDelete(NULL);
}