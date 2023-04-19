#include "ModelMeasure.h"



void ModelMeasure::set_barycentric(double x, double y, double z)
{
    this->barycentric_position.set_x(x);
    this->barycentric_position.set_y(y);
    this->barycentric_position.set_z(z);
}

void ModelMeasure::set_barycentric(BarycentricCoord coordinates)
{
    this->set_barycentric(coordinates.get_x(), coordinates.get_y(), coordinates.get_z());
}

void ModelMeasure::set_spherical(SphericalCoord coord)
{
    this->spherical_position.set_spherical(coord.get_right_ascension(), coord.get_declination());
}


void ModelMeasure::set_velocity(double vx, double vy, double vz)
{
    this->velocity.set_all_velocity(vx, vy, vz);
}

void ModelMeasure::set_velocity(Velocity v)
{
    this->set_velocity(v.get_vx(), v.get_vy(), v.get_vz());
}


void ModelMeasure::set_date(Date date)
{
    this->date = date;
}


BarycentricCoord ModelMeasure::get_barycentric()
{
    return this->barycentric_position;
}

SphericalCoord ModelMeasure::get_spherical()
{
    return this->spherical_position;
}




Velocity ModelMeasure::get_velocity()
{
    return this->velocity;
}


Date ModelMeasure::get_date()
{
    return this->date;
}
