#pragma once
#include <string>
#include "../../Helpers.h"


/*
    This class stores celestial coordinates, has methods for initializingand getting data,
    and interprets strings into coordinates.
    Used to store declination and right ascension {one object is declination or right ascension value}
*/
class CelestialCoord
{
private:
    double hours = 0;
    double minutes = 0;
    double seconds = 0;
    Helpers help;
public:
    CelestialCoord() = default;
    CelestialCoord(double h, double m, double s) : hours(h), minutes(m), seconds(s) {};

    //getters
    double get_hours();
    double get_minutes();
    double get_seconds();

    //setters
    void set_from_string(std::string);

    //operators
    CelestialCoord(const CelestialCoord& other);
    CelestialCoord& operator=(const CelestialCoord& other);
    CelestialCoord(const CelestialCoord&& other);
    CelestialCoord& operator=(const CelestialCoord&& other);
};
