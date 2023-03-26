#pragma once
#include "../coords/BarycentricCoord.h"
#include "../coords/SphericalCoord.h"

#include "../movement/Velocity.h"

#include "../time/Date.h"


/*
    A vector of values used in the numerical integration of the problem.
    The problem is described by the following system of differential equations:

    X(t) = (x, y, z, vx, vy, vz)
    X(0) = (x0, y0, z0, vx0, vy0, vz0)
    dX/dt = (vx, vy, vz, ax, ay, az)

    @param spherical_position - position of the Oumuamua in the Spherical coordinates
    @param barycentric_position (param x, y, z in the equations) - position of the Oumuamua in the Barycentric coordinates
    @param velocity (param vx, vy, vx in the equations) - velocity of the Oumuamua 
    @param date - time, at a given moment of which the position of Oumuamua is described

   The parameters ax,ay,az are implemented in the class with numerical integration (Integration) and are described there
*/
class IntegrationVector
{
private:
    BarycentricCoord barycentric_position;
    SphericalCoord spherical_position;
    Velocity velocity;
    //@change julian_date -> date
    Date date;
public:
    IntegrationVector() = default;


    //setters
    //@change set_porition -> set_barycentric_position
    void set_barycentric_position(double, double, double);
    void set_spherical_position(double, double);
    void set_velocity(double, double, double);
    //@change set_date -> set_date
    void set_date(Date);


    //getters
    //@change get_position -> get_barycentric_position
    BarycentricCoord get_barycentric_position();
    SphericalCoord get_spherical_position();
    Velocity get_velocity();
    //@change Date* get_date -> Date get_date
    Date get_date();


    /*
        Operation between integration vectors
    */
    friend IntegrationVector operator+(IntegrationVector, double);
    friend IntegrationVector operator+(IntegrationVector, IntegrationVector);
    friend IntegrationVector operator*(double, IntegrationVector);
};