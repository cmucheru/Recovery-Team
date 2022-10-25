#include "NKJLoRa.h"
#include <Ejection.h>
#include <Sensors.h>
#include <FlightStatus.h>
#include <Tasks.h>

const uint8_t LORA_SPREADING_FACTOR = 7;

void initHeltecLoRa()
{
    // disable OLED initialization
    Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa disable*/, true /*Serial Enable */, true /* PABOOST Enable*/, FREQUENCY_BAND /*Long Band*/);

    // Set LoRa Spreading Fator
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
}
char *printTransmitMessageLoRa(GPSReadings gpsReadings)
{
    // The assigned size is calculated to fit the string
    char *message = (char *)pvPortMalloc(15);

    if (!message)
        return NULL;

    snprintf(message, 15, "{%.7f,%.7f}\n", gpsReadings.latitude, gpsReadings.longitude);
    return message;
}
void sendLora(GPSReadings gpsReadings)
{
    LoRa.beginPacket();
    char *message = printTransmitMessageLoRa(gpsReadings);
    LoRa.print(message);
    // send packet
    if (LoRa.endPacket())
    {
        debugln(message);
    }
    vPortFree(message);
}
char *printTransmitMessageLoRa(FlightStatus flightStatus)
{
    // The assigned size is calculated to fit the string
    char *message = (char *)pvPortMalloc(25);

    if (!message)
        return NULL;

    snprintf(message, 25, "{%d,%d,%d,%d,%d,%d,%d}\n", flightStatus.isLaunched, flightStatus.isPrimaryDrogueFired, flightStatus.isPrimaryMainFired, flightStatus.isDrogueSeparated, flightStatus.isMainSeparated, flightStatus.isManualDrogueFired, flightStatus.isManualMainFired);
    return message;
}

void sendLora(FlightStatus flightStatus)
{
    LoRa.beginPacket();
    char *message = printTransmitMessageLoRa(flightStatus);
    LoRa.print(message);
    // send packet
    if (LoRa.endPacket())
    {
        debugln(message);
    }
    vPortFree(message);
}
