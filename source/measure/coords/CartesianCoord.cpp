#include "CartesianCoord.h"


double CartesianCoord::get_x()
{
    return this->x;
}

double CartesianCoord::get_y()
{
    return this->y;
}

double CartesianCoord::get_z()
{
    return this->z;
}

void CartesianCoord::set_x(double value)
{
    this->x = value;
}


void CartesianCoord::set_y(double value)
{
    this->y = value;
}


void CartesianCoord::set_z(double value)
{
    this->z = value;
}

void CartesianCoord::print()
{
    std::cout << "x= " << this->x << " y= " << this->y << " z= " << this->z << "\n";
}
