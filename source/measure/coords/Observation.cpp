#include "Observation.h"


Date* Observation::get_date()
{
    return &date;
}


std::string Observation::get_code() 
{
    return this->observatory_code;
}


void Observation::set_date(Date date) 
{
    this->date = date;
}


void Observation::set_code(std::string code)
{
    this->observatory_code = code;
}


void Observation::set_ascension_from_string(std::string value) 
{
    this->spherical_position.set_RA_in_hours_system_from_string(value);
}


void Observation::set_declination_from_string(std::string value) 
{
    this->spherical_position.set_DEC_in_hours_system_from_string(value);
}


void Observation::set_spherical(double longitude, double latitude) 
{
    this->spherical_position.set_right_ascension(longitude);
    this->spherical_position.set_declination(latitude);
}


void Observation::set_barycentric(double alpha, double beta, double gamma) 
{
    this->barycentric_position.set_alpha(alpha);
    this->barycentric_position.set_beta(beta);
    this->barycentric_position.set_gamma(gamma);
}

void Observation::set_barycentric(BarycentricCoord coordinates)
{
    this->set_barycentric(coordinates.get_alpha(), coordinates.get_beta(), coordinates.get_gamma());
}


void Observation::set_geocentric(double x, double y, double z)
{
    this->geocentric_position.set_x(x);
    this->geocentric_position.set_y(y);
    this->geocentric_position.set_z(z);
}


SphericalCoord Observation::get_spherical_position()
{
    return this->spherical_position;
}


GeocentricCoord Observation::get_geocentric()
{
    return this->geocentric_position;
}


BarycentricCoord Observation::get_barycentric() 
{
    return this->barycentric_position;
}