#include "IntegrationVector.h"

void IntegrationVector::set_position(double x, double y, double z)
{
    this->position.set_x(x);
    this->position.set_y(y);
    this->position.set_z(z);
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

Velocity IntegrationVector::get_velocity()
{
    return velocity;
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

//IntegrationVector operator*(double delta, IntegrationVector vector) 
//{
//    IntegrationVector result = vector;
//    result.position.multiply(delta);
//    result.velocity.multiply(delta);
//    return result;
//}

//IntegrationVector::IntegrationVector(const IntegrationVector& other)
//{
//    this->position = other.position;
//    this->velocity = other.velocity;
//}



Date IntegrationVector::get_julian_date()
{
    return julian_date;
}


//IntegrationVector::IntegrationVector(const IntegrationVector& other)
//{
//    this->position = other.position;
//    this->velocity = other.velocity;
//}

//IntegrationVector& IntegrationVector::operator=(const IntegrationVector& other) 
//{
//    this->position = other.position;
//    this->velocity = other.velocity;
//    return *this;
//}



IntegrationVector operator*(double delta, IntegrationVector vector)
{
    //IntegrationVector result = vector;
    IntegrationVector result;
    double x = vector.get_position().get_x() * delta;
    double y = vector.get_position().get_y() * delta;
    double z = vector.get_position().get_z() * delta;
    result.set_position(x, y, z);
    double vx = vector.get_velocity().get_vx() * delta;
    double vy = vector.get_velocity().get_vy() * delta;
    double vz = vector.get_velocity().get_vz() * delta;
    result.set_velocity(vx, vy, vz);
    return result;
}