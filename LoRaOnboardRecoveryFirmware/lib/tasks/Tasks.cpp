#include "Sensors.h"
#include <NKJLoRa.h>
#include <FlightStatus.h>
#include <Tasks.h>
#include <Ejection.h>

extern QueueHandle_t gps_queue;
extern QueueHandle_t flight_status_queue;

extern TaskHandle_t GetGPSTaskHandle;
extern TaskHandle_t SendGPSLoRaTaskHandle;

// Pins to start ejection charge
extern uint8_t MAIN_EJECTION_PIN;
extern uint8_t DROGUE_EJECTION_PIN;

extern char DROGUE_MESSAGE[];
extern char MAIN_MESSAGE[];

void readGPSTask(void *parameter)
{
    vTaskSuspend(NULL);
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
void sendGPSLoRaTask(void *parameter)
{
    vTaskSuspend(NULL);
    struct GPSReadings gpsReadings = {0};
    float latitude = 0;
    float longitude = 0;

    for (;;)
    {
        gpsReadings.latitude = latitude;
        gpsReadings.longitude = longitude;

        if (xQueueReceive(gps_queue, (void *)&gpsReadings, 10) == pdTRUE)
        {
            if (gpsReadings.longitude != 0 && gpsReadings.latitude != 0)
            {
                latitude = gpsReadings.latitude;
                longitude = gpsReadings.longitude;
            }
            sendLora(gpsReadings);
            LoRa.receive();
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void getStatusTask(void *parameter)
{
    struct FlightStatus flightStatus = {0};
    static int droppedFlightStatusPackets = 0;
    for (;;)
    {
        flightStatus = get_flight_status();
        if (xQueueSend(flight_status_queue, (void *)&flightStatus, 0) != pdTRUE)
        {
            droppedFlightStatusPackets++;
        }

        // debugf("Dropped FlightStatus Packets : %d\n", droppedFlightStatusPackets);

        // yield to idle task
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void sendStatusLoRaTask(void *parameter)
{
    struct FlightStatus flightStatus = {0};
    for (;;)
    {
        if (xQueueReceive(flight_status_queue, (void *)&flightStatus, 10) == pdTRUE)
        {
            sendLora(flightStatus);
            LoRa.receive();
        }
        // yield to idle task
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void OnReceiveTask(void *param)
{
    for (;;)
    {
        LoRa.receive();
        int packetSize = LoRa.parsePacket();
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
            //resumeGPSTasks();
        }
        debugln(LoRa.packetRssi());
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void resumeGPSTasks()
{
    vTaskResume(GetGPSTaskHandle);
    vTaskResume(SendGPSLoRaTaskHandle);
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
        //resumeGPSTasks();
    }
    debugln(LoRa.packetRssi());
}