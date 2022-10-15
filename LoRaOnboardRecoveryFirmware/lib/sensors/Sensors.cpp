#include "Sensors.h"
#include <TinyGPS++.h>

TinyGPSPlus gps;

void init_gps()
{
    Serial2.begin(GPS_BAUD_RATE, SERIAL_8N1, GPS_TX_PIN, GPS_RX_PIN);
}
struct GPSReadings get_gps_readings()
{
    struct GPSReadings gpsReadings = {0};
    while (Serial2.available() > 0)
    {
        if (gps.encode(Serial2.read()))
        {
            if (gps.location.isValid())
            {
                gpsReadings.latitude = gps.location.lat();
                gpsReadings.longitude = gps.location.lng();
            }
        }
    }

    return gpsReadings;
}