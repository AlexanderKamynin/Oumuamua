#include "IntegrationVector.h"


void IntegrationVector::set_barycentric_position(double x, double y, double z)
{
    this->barycentric_position.set_x(x);
    this->barycentric_position.set_y(y);
    this->barycentric_position.set_z(z);
}


//void IntegrationVector::set_spherical_position(double RA, double DEC)
//{
//    this->spherical_position.set_right_ascension(RA);
//    this->spherical_position.set_declination(DEC);
//}


void IntegrationVector::set_velocity(double vx, double vy, double vz) 
{
    this->velocity.set_all(vx, vy, vz);
}


void IntegrationVector::set_date(Date date) 
{
    this->date = date;
}


BarycentricCoord IntegrationVector::get_barycentric_position() 
{
    return this->barycentric_position;
}


//SphericalCoord IntegrationVector::get_spherical_position() 
//{
//    return this->spherical_position;
//}


Velocity IntegrationVector::get_velocity() 
{
    return this->velocity;
}


Date IntegrationVector::get_date() 
{
    return this->date;
}


IntegrationVector operator+(IntegrationVector vector, double value)
{
    //@TODO increase for spherical coord ????
    vector.barycentric_position.increase(value);
    vector.velocity.increase(value);
    return vector;
}


IntegrationVector operator+(IntegrationVector vector1, IntegrationVector vector2)
{
    IntegrationVector result;
    result.barycentric_position = vector1.barycentric_position + vector2.barycentric_position;
    result.velocity = vector1.velocity + vector2.velocity;
    result.date = vector1.date;
    return result;
}


IntegrationVector operator*(double value, IntegrationVector vector)
{
    vector.barycentric_position.multiply(value);
    vector.velocity.multiply(value);
    return vector;
}

