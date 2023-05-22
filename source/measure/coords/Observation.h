#pragma once
#include "SphericalCoord.h"
#include "BarycentricCoord.h"
#include "GeocentricCoord.h"
#include "../movement/Velocity.h"
#include "../time/Date.h"

#include <string>


/*
    This class represents objects that store information about the position of Oumuamua
    in different coordinate systems at a point in time.
*/
class Observation 
{
private:
    Date date;
    Date TT;
    double TDB; // in MJD format
    BarycentricCoord observatory_position;
    std::string observatory_code;
    SphericalCoord spherical_position;
    GeocentricCoord geocentric_position;
    BarycentricCoord barycentric_position;
    Velocity velocity;
public:
    Observation() = default;

    //setters
    void set_date(Date);
    void set_code(std::string);
    void set_RA_in_hours_systems(double, double, double);
    void set_DEC_in_degrees_systems(double, double, double);
    void set_spherical(double, double);
    void set_spherical(SphericalCoord);
    void set_geocentric(double, double, double);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(double, double, double);
    void set_barycentric(BarycentricCoord);
    void set_observatory_position(BarycentricCoord);
    void set_velocity(Velocity);
    void set_TT(Date);
    void set_TDB(double);

    //getters
    Date* get_date();
    Date* get_TT();
    double get_TDB();
    std::string get_code();
    SphericalCoord get_spherical_position();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
    BarycentricCoord get_observatory_position();
    Velocity get_velocity();
};