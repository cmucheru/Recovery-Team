
#include "heltec.h"
#include "../include/defs.h"
#include "../include/initlora.h"

void setup() {
  
  initHeltecLoRa();
  
}

void loop() {
  
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    // read packet
    while (LoRa.available()) {
      debugln((char)LoRa.read());
    }               
}
}