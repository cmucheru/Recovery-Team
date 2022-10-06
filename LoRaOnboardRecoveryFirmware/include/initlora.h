#ifndef INITLORA_H
#define INITLORA_H

#include "initlora.h"
#include "defs.h"
#include "heltec.h"


void initHeltecLoRa()
{
    //disable OLED initialization
    Heltec.begin(false /*DisplayEnable Enable*/, true /*Heltec.LoRa disable*/, true /*Serial Enable */, true /* PABOOST Enable*/, FREQUENCY_BAND /*Long Band*/);

    //Set LoRa Spreading Fator
    LoRa.setSpreadingFactor(LORA_SPREADING_FACTOR);
}



#endif