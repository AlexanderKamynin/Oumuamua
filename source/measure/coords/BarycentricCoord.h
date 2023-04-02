#pragma once
#include <math.h>
#include "../../Helpers.h"


/*
    This class represents the coordinate system (related to the barycenter of the solar system)
    in which the celestial objects exist

    alpha, beta, gamma - coordinates (vectors) for this system

*/
class BarycentricCoord
{
private:
    double alpha = 0;
    double beta = 0;
    double gamma = 0;

    Helpers help;
public:
    BarycentricCoord() = default;
    BarycentricCoord(double a, double b, double g) : alpha(a), beta(b), gamma(g) {};

    //getters
    double get_alpha();
    double get_beta();
    double get_gamma();


    //setters
    void set_alpha(double);
    void set_beta(double);
    void set_gamma(double);


    /*
        Method for + and * all coordinates on one value
    */
    void increase(double);
    void multiply(double);


    double length();

    /*
        Operation between barycentric coordinates
    */
    friend BarycentricCoord operator+(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator-(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator*(double, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, double);
};
