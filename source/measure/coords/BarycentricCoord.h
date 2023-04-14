#pragma once
#include <math.h>
#include "../../Helpers.h"
#include <iostream>
#include <iomanip>


/*
    This class represents the coordinate system (related to the barycenter of the solar system)
    in which the celestial objects exist

    x, y, z - coordinates (vectors) for this system

*/
class BarycentricCoord
{
private:
    double x = 0;
    double y = 0;
    double z = 0;

    Helpers help;
public:
    BarycentricCoord() = default;
    BarycentricCoord(double x, double y, double z) : x(x), y(y), z(z) {};

    //getters
    double get_x();
    double get_y();
    double get_z();

    //setters
    void set_x(double);
    void set_y(double);
    void set_z(double);
    
    //set x, y, z
    void set_all_coords(double, double, double);

    void increase(double);
    void multiply(double);
    double length();

    void print();

    /*
        Operation between barycentric coordinates
    */
    friend BarycentricCoord operator+(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator-(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator*(BarycentricCoord, double);
    friend BarycentricCoord operator*(double, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, double);
};
