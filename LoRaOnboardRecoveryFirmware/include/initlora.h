#ifndef INITLORA_H
#define INITLORA_H

#include "initlora.h"
#include "defs.h"
#include "heltec.h"

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
        // debugln("Sent Lora Done");
    }
}

#endif