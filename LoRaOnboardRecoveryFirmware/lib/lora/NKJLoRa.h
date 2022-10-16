#ifndef NAKUJALORA_H
#define NAKUJALORA_H
#include "../definitions/defs.h"

#define FREQUENCY_BAND 915E6 // you can set band here directly,e.g. 868E6,915E6

struct SendValues
{
    float latitude;
    float longitude;
};

void initHeltecLoRa();
char *printTransmitMessageLoRa(SendValues sv);
void sendTelemetryLora(SendValues sv);
void onReceive(int packetSize);
#endif // NAKUJALORA_H