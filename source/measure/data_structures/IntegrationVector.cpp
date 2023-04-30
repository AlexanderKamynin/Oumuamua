#include "IntegrationVector.h"


void IntegrationVector::set_barycentric(double x, double y, double z)
{
    this->barycentric_position.set_x(x);
    this->barycentric_position.set_y(y);
    this->barycentric_position.set_z(z);
}

void IntegrationVector::set_barycentric(BarycentricCoord coordinates)
{
    this->set_barycentric(coordinates.get_x(), coordinates.get_y(), coordinates.get_z());
}


void IntegrationVector::set_velocity(double vx, double vy, double vz) 
{
    this->velocity.set_all_velocity(vx, vy, vz);
}

void IntegrationVector::set_velocity(Velocity v)
{
    this->set_velocity(v.get_vx(), v.get_vy(), v.get_vz());
}


void IntegrationVector::set_date(Date date) 
{
    this->date = date;
}

void IntegrationVector::set_dx_db(Matrix dx_db)
{
    this->dx_db = dx_db;
}


BarycentricCoord IntegrationVector::get_barycentric() 
{
    return this->barycentric_position;
}




Velocity IntegrationVector::get_velocity() 
{
    return this->velocity;
}


Date IntegrationVector::get_date() 
{
    return this->date;
}

Matrix* IntegrationVector::get_df_dx()
{
    return &this->df_dx;
}

Matrix* IntegrationVector::get_dx_db()
{
    return &this->dx_db;
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
    result.df_dx = (*vector1.get_df_dx()) + (*vector2.get_df_dx());
    result.dx_db = (*vector1.get_dx_db()) + (*vector2.get_dx_db());
    return result;
}


IntegrationVector operator*(double value, IntegrationVector vector)
{
    vector.barycentric_position.multiply(value);
    vector.velocity.multiply(value);
    vector.df_dx = value * (*vector.get_df_dx());
    vector.dx_db = value * (*vector.get_dx_db());
    return vector;
}

