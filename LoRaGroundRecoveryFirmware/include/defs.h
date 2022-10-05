#ifndef DEFS_H
#define DEFS_H

#define DEBUG 1
#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#define debugf(x, y) Serial.printf(x, y)
#else
#define debug(x)
#define debugln(x)
#define debugf(x, y)
#endif

#define FREQUENCY_BAND 915E6

const uint8_t GROUND_DROGUE_PIN = 23;
const uint8_t GROUND_MAIN_PIN = 36;

const char DROGUE_MESSAGE[] = "D";
const char MAIN_MESSAGE[] = "M";

const int LORA_SPREADING_FACTOR = 7;

const BaseType_t pro_cpu = 0;
const BaseType_t app_cpu = 1;

#endif