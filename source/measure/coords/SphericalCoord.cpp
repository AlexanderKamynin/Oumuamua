#include "SphericalCoord.h"


void SphericalCoord::set_longitude(double longitude)
{
    this->longitude = longitude;
}


void SphericalCoord::set_latitude(double latitude) 
{
    this->latitude = latitude;
}


double SphericalCoord::get_longitude()
{
    return longitude;
}


double SphericalCoord::get_latitude()
{
    return latitude;
}