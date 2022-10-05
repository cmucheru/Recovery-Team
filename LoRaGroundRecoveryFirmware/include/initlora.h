#ifndef INITLORA_H
#define INITLORA_H

#include "heltec.h"
#include "defs.h"

void initHeltecLoRa()
{
    //doesn't initialize OLED
    Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa Disable*/, true /*Serial Enable*/, true /*PABOOST Enable*/, FREQUENCY_BAND /*long BAND*/);

    // set spreading factor
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
}

#endif