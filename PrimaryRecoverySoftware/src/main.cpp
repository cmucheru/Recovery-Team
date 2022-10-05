#include <Arduino.h>
#include "../include/checkState.h"
#include "../include/logdata.h"
#include "../include/readsensors.h"
#include "../include/defs.h"
#include "../include/kalmanfilter.h"

TimerHandle_t ejectionTimerHandle = NULL;

TaskHandle_t WiFiTelemetryTaskHandle = NULL;
TaskHandle_t GetDataTaskHandle = NULL;
TaskHandle_t SDWriteTaskHandle = NULL;
TaskHandle_t GPSTaskHandle = NULL;

float BASE_ALTITUDE = 0;

float previousAltitude = 0.0;

int state = 0;

static uint16_t wifi_queue_length = 100;
static uint16_t sd_queue_length = 500;
static uint16_t gps_queue_length = 100;

static QueueHandle_t wifi_telemetry_queue;
static QueueHandle_t sdwrite_queue;
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

struct LogData readData()
{
    struct LogData ld = {0};
    struct SensorReadings readings = {0};
    struct FilteredValues filtered_values = {0};

    readings = get_readings();

    // TODO: very important to know the orientation of the altimeter
    filtered_values = kalmanUpdate(readings.altitude, readings.ay);
    float currentAltitude = filtered_values.displacement;
    debugln(currentAltitude);

    // Update state
    state = checkState(currentAltitude, previousAltitude, filtered_values.velocity, filtered_values.acceleration, state);

    previousAltitude = currentAltitude;

    ld = formart_SD_data(readings, filtered_values);
    ld.state = state;

    ld.timeStamp = millis();

    return ld;
}

/*
**********Time Taken for each Task******************
        Get Data Task  - 36ms
        WiFiTelemetryTask -74ms
        GPS Task - 1000ms
        SD Write Task - 60ms
*/

void GetDataTask(void *parameter)
{

    struct LogData ld = {0};

    for (;;)
    {

        ld = readData();

        // yield to idle task on core 0
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
void readGPSTask(void *parameter)
{

    struct GPSReadings gpsReadings = {0};

    for (;;)
    {
        //gpsReadings = get_gps_readings();
        // yield to idle task on core 1
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void setup()
{

    Serial.begin(BAUD_RATE);

    // set up ejection pins
    pinMode(DROGUE_EJECTION_PIN, OUTPUT);
    pinMode(MAIN_EJECTION_PIN, OUTPUT);
    ejection(DROGUE_EJECTION_PIN);
    init_sensors();

    // get the base_altitude
    BASE_ALTITUDE = get_base_altitude();

    // initialize core tasks
    xTaskCreatePinnedToCore(GetDataTask, "GetDataTask", 3000, NULL, 1, &GetDataTaskHandle, pro_cpu);
    xTaskCreatePinnedToCore(readGPSTask, "ReadGPSTask", 3000, NULL, 1, &GPSTaskHandle, app_cpu);

    vTaskDelete(NULL);
}
void loop()
{
}