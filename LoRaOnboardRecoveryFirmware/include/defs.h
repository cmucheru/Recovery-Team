#ifndef DEFINITIONS_H
#define DEFINITIONS_H



#include <Wire.h>

#define FREQUENCY_BAND    915E6  //you can set band here directly,e.g. 868E6,915E6


#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

#define SEA_LEVEL_PRESSURE 102400

// Timing delays
#define SETUP_DELAY 5000

#define SHORT_DELAY 10

#define BAUD_RATE 115200

#define GPS_BAUD_RATE 9600

#define SD_CS_PIN 5

// Pin to start ejection charge
const uint8_t MAIN_EJECTION_PIN= 23;
const uint8_t DROGUE_EJECTION_PIN= 12;

const uint8_t GPS_TX_PIN = 13;
const uint8_t GPS_RX_PIN = 37;

const BaseType_t pro_cpu = 0;
const BaseType_t app_cpu = 1;


//TODO: carry out tests to know how much time
const int EJECTION_PIN_ON_TIME_MS = 3000;


// This struct is used to save all our datapoints.
// It includes rocket altitude, accelerations in the x, y and z directions
// Gryroscope values in the x, y and z direcion
// filtered altitude, velocity and acceleration
// GPS longitude, laltitude and altitude and state
struct LogData
{
    uint64_t timeStamp;

    float latitude;
    float longitude;
    float gpsAltitude;
};

// GPSReadings contains the gps informations that is
// latitude, longitude, speed, number of satellites and altitude
struct GPSReadings
{
    float latitude;
    float longitude;
};


// SendValues contains the data points we will be sending over lora
struct SendValues
{
    uint64_t timeStamp;
    float altitude;
    uint16_t state;
    float latitude;
    float longitude;
};

#endif