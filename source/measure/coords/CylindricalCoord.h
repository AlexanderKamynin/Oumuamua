#pragma once
#include <string>


/*
    This class stores cylindrical coordinates,
    has methods for initializing and getting data,
    and interprets strings into coordinates.
    Used to store information about the position of observatories
*/
class CylindricalCoord
{
private:
    double longitude = 0;
    double cos = 0;
    double sin = 0;
public:
    CylindricalCoord() = default;

    //getters
    double get_longitude();
    double get_cos();
    double get_sin();

    //setters
    void set_longitude_from_string(std::string value);
    void set_cos_from_string(std::string value);
    void set_sin_from_string(std::string value);

    //operators
    CylindricalCoord(const CylindricalCoord& other);
    CylindricalCoord& operator=(const CylindricalCoord& other);
    CylindricalCoord(const CylindricalCoord&& other);
    CylindricalCoord& operator=(const CylindricalCoord&& other);
};