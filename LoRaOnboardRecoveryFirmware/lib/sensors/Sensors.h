#ifndef SENSORS_H
#define SENSORS_H

#include "../definitions/defs.h"

#define GPS_BAUD_RATE 9600

// GPSReadings contains the gps informations that is
// latitude, longitude, speed, number of satellites and altitude
struct GPSReadings
{
    float latitude;
    float longitude;
};


struct GPSReadings get_gps_readings();
void init_gps();

#endif // SENSORS_H