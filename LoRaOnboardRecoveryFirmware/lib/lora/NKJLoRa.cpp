#include "NKJLoRa.h"
#include <Ejection.h>


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
char *printTransmitMessageLoRa(SendValues sv)
{
    // The assigned size is calculated to fit the string
    char *message = (char *)pvPortMalloc(60);

    if (!message)
        return NULL;

    snprintf(message, 60, "{\"latitude\":%.7f,\"longitude\":%.7f}\n", sv.latitude, sv.longitude);
    return message;
}
void sendTelemetryLora(SendValues sv)
{
    LoRa.beginPacket();
    char *message = printTransmitMessageLoRa(sv);
    LoRa.print(message);
    vPortFree(message);
    // send packet
    if (LoRa.endPacket())
    {
        //debugln("Sent Lora Done");
    }
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
    }
    debugln(LoRa.packetRssi());
}