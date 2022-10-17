#include "NKJLoRa.h"
#include <Ejection.h>
#include <Sensors.h>
#include <FlightStatus.h>
#include <Tasks.h>

const uint8_t LORA_SPREADING_FACTOR = 7;

// Pins to start ejection charge
extern uint8_t MAIN_EJECTION_PIN;
extern uint8_t DROGUE_EJECTION_PIN;

extern char DROGUE_MESSAGE[];
extern char MAIN_MESSAGE[];

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
    char *message = (char *)pvPortMalloc(40);

    if (!message)
        return NULL;

    snprintf(message, 40, "{\"latitude\":%.7f,\"longitude\":%.7f}\n", gpsReadings.latitude, gpsReadings.longitude);
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
        //debugln(message);
    }
    vPortFree(message);
}
char *printTransmitMessageLoRa(FlightStatus flightStatus)
{
    // The assigned size is calculated to fit the string
    char *message = (char *)pvPortMalloc(90);

    if (!message)
        return NULL;

    snprintf(message, 90, "{\"launched\":%d,\"drogueFired\":%d,\"mainFired\":%d,\"drogueSeparated\":%d,\"mainSeparated\":%d}\n", flightStatus.isLaunched, flightStatus.isPrimaryDrogueFired, flightStatus.isPrimaryMainFired, flightStatus.isDrogueSeparated, flightStatus.isMainSeparated);
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
        resumeGPSTasks();
    }
    debugln(LoRa.packetRssi());
}