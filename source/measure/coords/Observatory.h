#pragma once
#include "CartesianCoord.h"
#include "CylindricalCoord.h"
#include "GeocentricCoord.h"
#include "BarycentricCoord.h"

/*
    This class describes the location of observatories in different coordinate systems
*/
class Observatory 
{
private:
    CartesianCoord cartesian_coordinate;
    CylindricalCoord cylindrical_coordinate;
    GeocentricCoord geocentric_coordinate;
    BarycentricCoord barycentric_coordinate;
public:
    Observatory() {};

    //setters
    void set_cartesian(CartesianCoord);
    void set_cylindrical(CylindricalCoord);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    //getters
    CartesianCoord get_cartesian();
    CylindricalCoord get_cylindric();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();

    Observatory(const Observatory& other);
    Observatory& operator=(const Observatory& other);
    Observatory(const Observatory&& other);
    Observatory& operator=(const Observatory&& other);
};