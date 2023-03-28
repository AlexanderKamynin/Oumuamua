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
    //@change double x -> double alpha
    double alpha = 0;
    //@change double y -> double beta
    double beta = 0;
    //@change double z -> double gamma
    double gamma = 0;

    Helpers help;
public:
    BarycentricCoord() = default;


    //getters
    //@change double get_x() -> double get_alpha()
    double get_alpha();
    //@change double get_y() -> double get_beta()
    double get_beta();
    //@change double get_z() -> double get_gamma()
    double get_gamma();


    //setters
    //@change double set_x() -> double set_alpha()
    void set_alpha(double);
    //@change double set_y() -> double set_beta()
    void set_beta(double);
    //@change double set_z() -> double set_gamma()
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
    //@change  friend BarycentricCoord operator*(const double&, BarycentricCoord) -> friend BarycentricCoord operator*(double, BarycentricCoord)
    friend BarycentricCoord operator*(double, BarycentricCoord);
    friend BarycentricCoord operator/(BarycentricCoord, BarycentricCoord);
    //@change friend BarycentricCoord operator/(BarycentricCoord, const double&) -> friend BarycentricCoord operator/(BarycentricCoord, double);
    friend BarycentricCoord operator/(BarycentricCoord, double);
};
