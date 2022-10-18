#ifndef NAKUJALORA_H
#define NAKUJALORA_H
#include "../definitions/defs.h"
#include <Sensors.h>
#include <FlightStatus.h>

#define FREQUENCY_BAND 915E6 // you can set band here directly,e.g. 868E6,915E6

struct SendValues
{
    float latitude;
    float longitude;
};

void initHeltecLoRa();
char *printTransmitMessageLoRa(FlightStatus flightStatus);
void sendLora(FlightStatus flightStatus);
char *printTransmitMessageLoRa(GPSReadings gpsReadings);
void sendLora(GPSReadings gpsReadings);
#endif // NAKUJALORA_H