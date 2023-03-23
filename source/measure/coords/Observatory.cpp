#include "Observatory.h"

void Observatory::set_cartesian(CartesianCoord frame)
{
    cartesian_frame = frame;
}

void Observatory::set_cylindrical(CylindricalCoord frame) 
{
    cylindrical_frame = frame;
}

void Observatory::set_geocentric(GeocentricCoord frame) 
{
    geocentric_frame = frame;
}

void Observatory::set_barycentric(BarycentricCoord frame) 
{
    barycentric_frame = frame;
}

CartesianCoord Observatory::get_cartesian() 
{
    return cartesian_frame;
}

CylindricalCoord Observatory::get_cylindric() 
{
    return cylindrical_frame;
}

GeocentricCoord Observatory::get_geocentric() 
{
    return geocentric_frame;
}

BarycentricCoord Observatory::get_barycentric() 
{
    return barycentric_frame;
}

Observatory::Observatory(const Observatory& other) 
{
    this->cartesian_frame = other.cartesian_frame;
    this->cylindrical_frame = other.cylindrical_frame;
    this->geocentric_frame = other.geocentric_frame;
    this->barycentric_frame = other.barycentric_frame;
}

Observatory& Observatory::operator=(const Observatory& other) 
{
    this->cartesian_frame = other.cartesian_frame;
    this->cylindrical_frame = other.cylindrical_frame;
    this->geocentric_frame = other.geocentric_frame;
    this->barycentric_frame = other.barycentric_frame;

    return *this;
}

Observatory::Observatory(const Observatory&& other) 
{
    this->cartesian_frame = other.cartesian_frame;
    this->cylindrical_frame = other.cylindrical_frame;
    this->geocentric_frame = other.geocentric_frame;
    this->barycentric_frame = other.barycentric_frame;
}

Observatory& Observatory::operator=(const Observatory&& other)
{
    this->cartesian_frame = other.cartesian_frame;
    this->cylindrical_frame = other.cylindrical_frame;
    this->geocentric_frame = other.geocentric_frame;
    this->barycentric_frame = other.barycentric_frame;

    return *this;
}