#pragma once
#include <string>


//Class that interpreted celestian coordinate obj
class CelestialCoord {
private:
    double h = 0; // hours
    double m = 0; // minutes
    double s = 0; // seconds
public:
    CelestialCoord() = default;
    CelestialCoord(double h, double m, double s);

    //setters
    void set_from_string(std::string);

    //getters
    double get_h();
    double get_m();
    double get_s();
};