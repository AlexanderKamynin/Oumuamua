#pragma once
#include <string>


//Class that interpreted celestian coordinate obj
class CelestianCoord {
private:
    double h = 0; // hours
    double m = 0; // minutes
    double s = 0; // seconds
public:
    CelestianCoord() = default;
    CelestianCoord(double h, double m, double s);

    //setters
    void set_from_string(std::string);

    //getters
    double get_h();
    double get_m();
    double get_s();
};