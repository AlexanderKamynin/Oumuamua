#include "Observation.h"


Date* Observation::get_date()
{
    return &date;
}

Date* Observation::get_TT()
{
    return &TT;
}

double Observation::get_TDB()
{
    return this->TDB;
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


void Observation::set_RA_in_hours_systems(double h, double m, double s) 
{
    this->spherical_position.set_RA_in_hours_system_from_string(h, m, s);
}


void Observation::set_DEC_in_degrees_systems(double g, double m, double s) 
{
    this->spherical_position.set_DEC_in_degrees_system_from_string(g, m, s);
}


void Observation::set_spherical(double RA, double DEC) 
{
    this->spherical_position.set_right_ascension(RA);
    this->spherical_position.set_declination(DEC);
}


void Observation::set_spherical(SphericalCoord coordinates)
{
    this->set_spherical(coordinates.get_right_ascension(), coordinates.get_declination());
}


void Observation::set_barycentric(double x, double y, double z) 
{
    this->barycentric_position.set_x(x);
    this->barycentric_position.set_y(y);
    this->barycentric_position.set_z(z);
}

void Observation::set_barycentric(BarycentricCoord coordinates)
{
    this->set_barycentric(coordinates.get_x(), coordinates.get_y(), coordinates.get_z());
}

void Observation::set_observatory_position(BarycentricCoord position)
{
    this->observatory_position = position;
}

void Observation::set_velocity(Velocity v)
{
    this->velocity.set_all_velocity(v.get_vx(), v.get_vy(), v.get_vz());
}

void Observation::set_TT(Date TT)
{
    this->TT = TT;
}

void Observation::set_TDB(double tdb)
{
    this->TDB = tdb;
}


void Observation::set_geocentric(double x, double y, double z)
{
    this->geocentric_position.set_x(x);
    this->geocentric_position.set_y(y);
    this->geocentric_position.set_z(z);
}

void Observation::set_geocentric(GeocentricCoord coordinates)
{
    this->set_geocentric(coordinates.get_x(), coordinates.get_y(), coordinates.get_z());
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

BarycentricCoord Observation::get_observatory_position()
{
    return this->observatory_position;
}

Velocity Observation::get_velocity()
{
    return this->velocity;
}
