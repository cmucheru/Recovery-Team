#include "../lib/definitions/defs.h"
#include <Sensors.h>
#include <Ejection.h>
#include <NKJLoRa.h>
#include <Tasks.h>
#include <FlightStatus.h>
#include <Interrupts.h>

const BaseType_t pro_cpu = 0;
const BaseType_t app_cpu = 1;

TaskHandle_t SendGPSLoRaTaskHandle = NULL;
TaskHandle_t GetGPSTaskHandle = NULL;
TaskHandle_t GetFlightStatusTaskHandle = NULL;
TaskHandle_t SendFlightStatusLoRaTaskHandle = NULL;
TaskHandle_t OnReceiveTaskHandle = NULL;

QueueHandle_t gps_queue;
QueueHandle_t flight_status_queue;

const uint16_t GPS_QUEUE_LENGTH = 100;
const uint16_t FLIGHT_STATUS_QUEUE_LENGTH = 100;

void setup()
{
  initHeltecLoRa();

  setEjectionPinModes();

  LoRa.onReceive(onReceive);
  LoRa.receive();

  setInterruptPins();

  init_gps();

  // create GPS queue
  gps_queue = xQueueCreate(GPS_QUEUE_LENGTH, sizeof(GPSReadings));
  flight_status_queue = xQueueCreate(FLIGHT_STATUS_QUEUE_LENGTH, sizeof(FlightStatus));

  // Create tasks on core 1
  xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 2500, NULL, 1, &GetGPSTaskHandle, app_cpu);
  xTaskCreatePinnedToCore(getStatusTask, "GetFlightStatusTask", 2500, NULL, 1, &GetFlightStatusTaskHandle, app_cpu);

  // Create tasks on core 0
  //xTaskCreatePinnedToCore(sendGPSLoRaTask, "LoRaGPSTask", 2500, NULL, 1, &SendGPSLoRaTaskHandle, pro_cpu);
  //xTaskCreatePinnedToCore(sendStatusLoRaTask, "SendFlightStatusTask", 2500, NULL, 1, &SendFlightStatusLoRaTaskHandle, pro_cpu);
  //xTaskCreatePinnedToCore(OnReceiveTask, "OnReceiveTask", 1000, NULL, 1, &OnReceiveTaskHandle, pro_cpu);

  vTaskDelete(NULL);
}

void loop()
{
  vTaskDelete(NULL);
}