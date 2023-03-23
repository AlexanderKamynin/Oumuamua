#pragma once

#include "CelestialCoord.h"
#include "SphericalCoord.h"
#include "BarycentricCoord.h"
#include "GeocentricCoord.h"

#include "../time/Date.h"

#include <string>

//Класс для хранения наблюдения
class Observation 
{
private:
    Date julian_date;
    std::string code;
    CelestialCoord ascension;
    CelestialCoord declination;
    SphericalCoord spherical_position;
    GeocentricCoord geocentric_position;
    BarycentricCoord barycentric_position;

    double x = NULL;
    double y = NULL;
    double z = NULL;
public:
    Date* get_julian_date();
    std::string get_code();
    CelestialCoord get_ascension();
    CelestialCoord get_declination();
    void set_julian_date(Date value);
    void set_code(std::string value);
    void set_ascension(CelestialCoord value);
    void set_declination(CelestialCoord value);
    void set_ascension_from_string(std::string value);
    void set_declination_from_string(std::string value);
    void set_spherical(double longitude, double latitude);
    void set_geocentric(double, double, double);
    void set_barycentric(double, double, double);

    double get_x();
    double get_y();
    double get_z();
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);
    SphericalCoord get_spherical_position();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};