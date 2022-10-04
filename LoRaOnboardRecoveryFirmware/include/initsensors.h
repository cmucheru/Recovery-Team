#ifndef INITSENSORS_H
#define INITSENSORS_H
#include <SoftwareSerial.h>
#include "defs.h"

SoftwareSerial GPS_Serial(1);

SoftwareSerial GPSModule(GPS_RX_PIN, GPS_TX_PIN);

void init_gps()
{
    GPSModule.begin(GPS_BAUD_RATE);
}

String ConvertLat(String nmea[15])
{
    String posneg = "";
    if (nmea[3] == "S")
    {
        posneg = "-";
    }
    String latfirst;
    float latsecond;
    for (int i = 0; i < nmea[2].length(); i++)
    {
        if (nmea[2].substring(i, i + 1) == ".")
        {
            latfirst = nmea[2].substring(0, i - 2);
            latsecond = nmea[2].substring(i - 2).toFloat();
        }
    }
    latsecond = latsecond / 60;
    String CalcLat = "";

    char charVal[9];
    dtostrf(latsecond, 4, 6, charVal);
    for (int i = 0; i < sizeof(charVal); i++)
    {
        CalcLat += charVal[i];
    }
    latfirst += CalcLat.substring(1);
    latfirst = posneg += latfirst;
    return latfirst;
}

String ConvertLng(String nmea[15])
{
    String posneg = "";
    if (nmea[5] == "W")
    {
        posneg = "-";
    }

    String lngfirst;
    float lngsecond;
    for (int i = 0; i < nmea[4].length(); i++)
    {
        if (nmea[4].substring(i, i + 1) == ".")
        {
            lngfirst = nmea[4].substring(0, i - 2);
            // debugln(lngfirst);
            lngsecond = nmea[4].substring(i - 2).toFloat();
            // debugln(lngsecond);
        }
    }
    lngsecond = lngsecond / 60;
    String CalcLng = "";
    char charVal[9];
    dtostrf(lngsecond, 4, 6, charVal);
    for (int i = 0; i < sizeof(charVal); i++)
    {
        CalcLng += charVal[i];
    }
    lngfirst += CalcLng.substring(1);
    lngfirst = posneg += lngfirst;
    return lngfirst;
}

// Get the gps readings from the gps sensor
struct GPSReadings get_gps_readings()
{
    String nmea[15];
    int stringplace = 0;
    int pos = 0;
    struct GPSReadings gpsReadings={0};
    GPSModule.flush();
    while (GPSModule.available() > 0)
    {
        GPSModule.read();
    }
    if (GPSModule.find("$GPRMC,"))
    {
        debug("here");
        String tempMsg = GPSModule.readStringUntil('\n');
        for (int i = 0; i < tempMsg.length(); i++)
        {
            if (tempMsg.substring(i, i + 1) == ",")
            {
                nmea[pos] = tempMsg.substring(stringplace, i);
                stringplace = i + 1;
                pos++;
            }
            if (i == tempMsg.length() - 1)
            {
                nmea[pos] = tempMsg.substring(stringplace, i);
            }
        }
        float lati = ConvertLat(nmea).toFloat();
        float lngi = ConvertLng(nmea).toFloat();
        gpsReadings.latitude = lati;
        gpsReadings.longitude = lngi;
    }

    return gpsReadings;
}


#endif