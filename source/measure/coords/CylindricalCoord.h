#pragma once
#include <string>


class CylindricalCoord
{
private:
    double longitude = 0;
    double cos = 0; 
    double sin = 0; 
public:
    //getters
    double get_longitude();
    double get_cos();
    double get_sin();

    bool string_is_empty(std::string str);

    //setters
    void set_longitude_from_string(std::string value);
    void set_cos_from_string(std::string value);
    void set_sin_from_string(std::string value);
};

