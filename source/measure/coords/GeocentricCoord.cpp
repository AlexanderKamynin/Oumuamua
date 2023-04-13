#include "GeocentricCoord.h"


double GeocentricCoord::get_x()
{
    return this->x;
}


double GeocentricCoord::get_y()
{
    return this->y;
}


double GeocentricCoord::get_z()
{
    return this->z;
}


void GeocentricCoord::set_from_string(std::string coordinates)
{
    std::vector<double> vector_of_coords;
    vector_of_coords = help.split(coordinates, ' ', '\0');
    this->x = vector_of_coords[0];
    this->y = vector_of_coords[1];
    this->z = vector_of_coords[2];
}


void GeocentricCoord::set_x(double value)
{
    this->x = value;
}


void GeocentricCoord::set_y(double value)
{
    this->y = value;
}


void GeocentricCoord::set_z(double value)
{
    this->z = value;
}
