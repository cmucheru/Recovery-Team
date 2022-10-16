#include "Sensors.h"
#include <NKJLoRa.h>
extern QueueHandle_t gps_queue;

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

        // debugf("Dropped GPS Packets : %d\n", droppedGPSPackets);

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
            if (gpsReadings.longitude != 0 && gpsReadings.latitude != 0)
            {
                latitude = gpsReadings.latitude;
                longitude = gpsReadings.longitude;
            }
        }
        sendTelemetryLora(sv);
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}