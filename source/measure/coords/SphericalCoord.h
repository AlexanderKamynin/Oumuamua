#pragma once
#include <string>
#include <vector>
#include "../../Helpers.h"


/*
    This class is a coordinate system describing spherical coordinates on ñelectial sphere.
    The parameters are right ascension and declination.

    @param RA_in_hours_system describe right ascension in hours-system
    @param DEC_in_hours_system describe declination in hours-system
    @param right_ascension is degrees system
    @param declination in degrees system
*/
class SphericalCoord 
{
private:
    double RA_in_hours_system [3] = {0, 0, 0}; // hours, minutes, seconds
    double DEC_in_hours_system[3] = { 0, 0, 0 }; // hours, minutes, seconds

    double right_ascension = 0;
    double declination = 0;

    Helpers help;
public:
    SphericalCoord() = default;

    //setters
    void set_right_ascension(double);
    void set_declination(double);
    void set_RA_in_hours_system_from_string(std::string);
    void set_DEC_in_hours_system_from_string(std::string);

    //getters
    double get_right_ascension();
    double get_declination();
    double* get_RA_in_hours_system();
    double* get_DEC_in_hours_system();
};