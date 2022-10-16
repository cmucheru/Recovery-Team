#include <GNDLoRa.h>

const int LORA_SPREADING_FACTOR = 7;

extern uint8_t GROUND_DROGUE_PIN;
extern uint8_t GROUND_MAIN_PIN;

extern char DROGUE_MESSAGE[];
extern char MAIN_MESSAGE[];

void initHeltecLoRa()
{
    // doesn't initialize OLED
    Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, FREQUENCY_BAND /*long BAND*/);

    // set spreading factor
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
}
void sendLoRaCommand(uint8_t chutePin)
{
    LoRa.beginPacket();
    // send D if drogue , send M if main
    if (chutePin == GROUND_DROGUE_PIN)
    {
        LoRa.print(DROGUE_MESSAGE);
        if (LoRa.endPacket())
        {
            debugf("Sent %s\n", DROGUE_MESSAGE);
        }
    }
    else if (chutePin == GROUND_MAIN_PIN)
    {
        LoRa.print(MAIN_MESSAGE);

        if (LoRa.endPacket())
        {
            debugf("Sent %s\n", MAIN_MESSAGE);
        }
    }
}
void getDownStreamGPS(){
    int packetSize = LoRa.parsePacket();
    char gpsString[60];
    for (int i = 0; i < packetSize; i++)
    {
        gpsString[i] = (char)LoRa.read();
    }
    debugln(gpsString);
}