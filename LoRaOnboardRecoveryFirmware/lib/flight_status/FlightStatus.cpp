#include <FlightStatus.h>

bool isDrogueSeparated = false;
bool isMainSeparated = false;
bool isLaunched = false;
bool isPrimaryDrogueFired = false;
bool isPrimaryMainFired = false;

struct FlightStatus get_flight_status()
{
    struct FlightStatus flightStatus = {0};

    flightStatus.isDrogueSeparated = isDrogueSeparated;
    flightStatus.isMainSeparated = isMainSeparated;
    flightStatus.isPrimaryDrogueFired = isPrimaryDrogueFired;
    flightStatus.isPrimaryMainFired = isPrimaryMainFired;
    flightStatus.isLaunched = isLaunched;

    return flightStatus;
}