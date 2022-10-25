#ifndef FLIGHTSTATUS_H
#define FLIGHTSTATUS_H

#include "../definitions/defs.h"

struct FlightStatus
{
    bool isLaunched;
    bool isPrimaryDrogueFired;
    bool isPrimaryMainFired;
    bool isMainSeparated;
    bool isDrogueSeparated;
    bool isManualMainFired;
    bool isManualDrogueFired;
};

struct FlightStatus get_flight_status();

#endif // FLIGHTSTATUS_H