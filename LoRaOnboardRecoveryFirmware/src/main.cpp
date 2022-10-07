
#include "heltec.h"
#include "../include/defs.h"
#include "../include/initlora.h"
#include "../include/initsensors.h"

TaskHandle_t GPSTaskHandle = NULL;

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

void setup()
{

  initHeltecLoRa();

  init_gps();

  // Create tasks on core 1
  xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 3000, NULL, 1, &GPSTaskHandle, app_cpu);

  vTaskDelete(NULL);
}

void loop()
{
}