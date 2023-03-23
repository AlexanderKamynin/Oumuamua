#include "IntegrationVector.h"


void IntegrationVector::set_position(double x, double y, double z)
{
    this->position.set_x(x);
    this->position.set_y(y);
    this->position.set_z(z);
}

void IntegrationVector::set_spherical_position(double longitude, double latitude)
{
    this->spherical_position.set_longitude(longitude);
    this->spherical_position.set_latitude(latitude);
}

void IntegrationVector::set_velocity(double vx, double vy, double vz) 
{
    this->velocity.set(vx, vy, vz);
}

void IntegrationVector::set_julian_date(Date date) 
{
    julian_date = date;
}

BarycentricCoord IntegrationVector::get_position() 
{
    return position;
}

SphericalCoord IntegrationVector::get_spherical_position() 
{
    return spherical_position;
}

Velocity IntegrationVector::get_velocity() 
{
    return velocity;
}

Date* IntegrationVector::get_julian_date() 
{
    return &julian_date;
}

IntegrationVector operator+(IntegrationVector vector, double delta)
{
    IntegrationVector result = vector;
    result.position.increase(delta);
    result.velocity.increase(delta);
    return result;
}

IntegrationVector operator+(IntegrationVector vector, IntegrationVector delta)
{
    IntegrationVector result;
    result.position = vector.position + delta.position;
    result.velocity = vector.velocity + delta.velocity;
    return result;
}

IntegrationVector operator*(double delta, IntegrationVector vector)
{
    IntegrationVector result = vector;
    result.position.multiply(delta);
    result.velocity.multiply(delta);
    return result;
}

IntegrationVector::IntegrationVector(const IntegrationVector& other)
{
    this->position = other.position;
    this->velocity = other.velocity;
    this->spherical_position = other.spherical_position;
    this->julian_date = other.julian_date;
}

IntegrationVector& IntegrationVector::operator=(const IntegrationVector& other) 
{
    this->position = other.position;
    this->velocity = other.velocity;
    this->spherical_position = other.spherical_position;
    this->julian_date = other.julian_date;
    return *this;
}

IntegrationVector::IntegrationVector(const IntegrationVector&& other)
{
    this->position = other.position;
    this->velocity = other.velocity;
    this->spherical_position = other.spherical_position;
    this->julian_date = other.julian_date;
}

IntegrationVector& IntegrationVector::operator=(const IntegrationVector&& other)
{
    this->position = other.position;
    this->velocity = other.velocity;
    this->spherical_position = other.spherical_position;
    this->julian_date = other.julian_date;
    return *this;
}