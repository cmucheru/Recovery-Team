#include <FlightStatus.h>

struct FlightStatus get_flight_status()
{
    struct FlightStatus flightStatus = {0};

    flightStatus.isDrogueSeparated = isDrogueSeparated;
    flightStatus.isMainSeparated = isMainSeparated;
    flightStatus.isPrimaryDrogueFired = isPrimaryDrogueFired;
    flightStatus.isPrimaryMainFired = isPrimaryMainFired;
    flightStatus.isLaunched = isLaunched;
    flightStatus.isManualDrogueFired = isManualDrogueFired;
    flightStatus.isManualMainFired = isManualMainFired;

    return flightStatus;
}