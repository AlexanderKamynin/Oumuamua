#include "Observation.h"

Date* Observation::get_julian_date()
{
    return &julian_date;
}

std::string Observation::get_code() 
{
    return code;
}

CelestialCoord Observation::get_ascension() 
{
    return ascension;
}

CelestialCoord Observation::get_declination() 
{
    return declination;
}

void Observation::set_julian_date(Date value) 
{
    julian_date = value;
}

void Observation::set_code(std::string value)
{
    code = value;
}

void Observation::set_ascension(CelestialCoord value)
{
    ascension = value;
}

void Observation::set_declination(CelestialCoord value) 
{
    declination = value;
}

void Observation::set_ascension_from_string(std::string value) 
{
    ascension.set_from_string(value);
}

void Observation::set_declination_from_string(std::string value) 
{
    declination.set_from_string(value);
}

void Observation::set_spherical(double longitude, double latitude) 
{
    spherical_position.set_latitude(latitude);
    spherical_position.set_longitude(longitude);
}

//@change void Observation::set_barycentric(double x, double y, double z) -> void Observation::set_barycentric(double alpha, double beta, double gamma)
void Observation::set_barycentric(double alpha, double beta, double gamma) 
{
    this->barycentric_position.set_alpha(alpha);
    this->barycentric_position.set_beta(beta);
    this->barycentric_position.set_gamma(gamma);
}

void Observation::set_geocentric(double x, double y, double z)
{
    geocentric_position.set_x(x);
    geocentric_position.set_y(y);
    geocentric_position.set_z(z);
}

double Observation::get_x()
{
    return x;
}

double Observation::get_y()
{
    return y;
}

double Observation::get_z() 
{
    return z;
}

void Observation::set_x(double value) 
{
    x = value;
}

void Observation::set_y(double value)
{
    y = value;
}

void Observation::set_z(double value) 
{
    z = value;
}

SphericalCoord Observation::get_spherical_position()
{
    return spherical_position;
}

GeocentricCoord Observation::get_geocentric()
{
    return geocentric_position;
}

BarycentricCoord Observation::get_barycentric() 
{
    return this->barycentric_position;
}