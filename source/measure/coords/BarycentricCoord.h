#pragma once
#include <math.h>

/*
    This class represents the coordinate system (related to the barycenter of the solar system)
    in which the celestial objects exist
*/
class BarycentricCoord
{
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    BarycentricCoord() = default;
    double get_x();
    double get_y();
    double get_z();
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);


    void increase(double);
    void multiply(double);
    double len();

    friend BarycentricCoord operator+(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator-(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator*(const double&, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, const double&);
};
