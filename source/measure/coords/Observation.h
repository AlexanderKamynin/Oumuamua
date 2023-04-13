#pragma once
#include "SphericalCoord.h"
#include "BarycentricCoord.h"
#include "GeocentricCoord.h"

#include "../time/Date.h"

#include <string>


/*
    This class represents objects that store information about the position of Oumuamua
    in different coordinate systems at a point in time.
*/
class Observation 
{
private:
    Date date;
    std::string observatory_code;
    SphericalCoord spherical_position;
    GeocentricCoord geocentric_position;
    BarycentricCoord barycentric_position;
public:
    Observation() = default;

    //setters
    void set_date(Date);
    void set_code(std::string);
    void set_ascension_from_string(std::string);
    void set_declination_from_string(std::string);
    void set_spherical(double, double);
    void set_spherical(SphericalCoord);
    void set_geocentric(double, double, double);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(double, double, double);
    void set_barycentric(BarycentricCoord);

    //getters
    Date* get_date();
    std::string get_code();
    SphericalCoord get_spherical_position();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};