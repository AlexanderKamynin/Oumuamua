#include "Observatory.h"


void Observatory::set_cartesian(CartesianCoord coord)
{
    this->cartesian_coordinate = coord;
}


void Observatory::set_cylindrical(CylindricalCoord coord)
{
    this->cylindrical_coordinate = coord;
}


void Observatory::set_geocentric(GeocentricCoord coord)
{
    this->geocentric_coordinate = coord;
}


void Observatory::set_barycentric(BarycentricCoord coord)
{
    this->barycentric_coordinate = coord;
}


CartesianCoord Observatory::get_cartesian() 
{
    return this->cartesian_coordinate;
}


CylindricalCoord Observatory::get_cylindric() 
{
    return this->cylindrical_coordinate;
}


GeocentricCoord Observatory::get_geocentric() 
{
    return this->geocentric_coordinate;
}


BarycentricCoord Observatory::get_barycentric() 
{
    return this->barycentric_coordinate;
}


Observatory::Observatory(const Observatory& other) 
{
    this->cartesian_coordinate = other.cartesian_coordinate;
    this->cylindrical_coordinate = other.cylindrical_coordinate;
    this->geocentric_coordinate = other.geocentric_coordinate;
    this->barycentric_coordinate = other.barycentric_coordinate;
}


Observatory& Observatory::operator=(const Observatory& other) 
{
    this->cartesian_coordinate = other.cartesian_coordinate;
    this->cylindrical_coordinate = other.cylindrical_coordinate;
    this->geocentric_coordinate = other.geocentric_coordinate;
    this->barycentric_coordinate = other.barycentric_coordinate;

    return *this;
}


Observatory::Observatory(const Observatory&& other) 
{
    this->cartesian_coordinate = other.cartesian_coordinate;
    this->cylindrical_coordinate = other.cylindrical_coordinate;
    this->geocentric_coordinate = other.geocentric_coordinate;
    this->barycentric_coordinate = other.barycentric_coordinate;
}


Observatory& Observatory::operator=(const Observatory&& other)
{
    this->cartesian_coordinate = other.cartesian_coordinate;
    this->cylindrical_coordinate = other.cylindrical_coordinate;
    this->geocentric_coordinate = other.geocentric_coordinate;
    this->barycentric_coordinate = other.barycentric_coordinate;

    return *this;
}