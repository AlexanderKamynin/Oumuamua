#pragma once
#include <string>
#include "../../Helpers.h"


/*
    This class stores geocentric coordinates,
    has methods for initializing and getting data,
    and interprets strings into coordinates.
*/
class GeocentricCoord
{
private:
    double x = 0;
    double y = 0;
    double z = 0;
    Helpers help;
public:
    GeocentricCoord() = default;

    //getters
    double get_x();
    double get_y();
    double get_z();

    //setters
    void set_from_string(std::string);
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);

    //operators
    GeocentricCoord(const GeocentricCoord& other);
    GeocentricCoord& operator=(const GeocentricCoord& other);
    GeocentricCoord(const GeocentricCoord&& other);
    GeocentricCoord& operator=(const GeocentricCoord&& other);
};