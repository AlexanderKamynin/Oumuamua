#pragma once
#include "BarycentricCoord.h"
#include "Velocity.h"
#include "Date.h"

class IntegrationVector 
{
private:
    BarycentricCoord position;
    Velocity velocity;
    Date julian_date;
public:
    IntegrationVector() {};

    void set_position(double, double, double);
    void set_velocity(double, double, double);
    void set_julian_date(Date);

    BarycentricCoord get_position();
    Velocity get_velocity();
    Date get_julian_date();

    friend IntegrationVector operator+(IntegrationVector, double);
    friend IntegrationVector operator+(IntegrationVector, IntegrationVector);
    friend IntegrationVector operator*(double, IntegrationVector);
};