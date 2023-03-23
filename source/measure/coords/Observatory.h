#pragma once
#include "CartesianCoord.h"
#include "CylindricalCoord.h"
#include "GeocentricCoord.h"
#include "BarycentricCoord.h"

//Класс для хранения данных обсерватории
class Observatory 
{
private:
    CartesianCoord cartesian_frame;
    CylindricalCoord cylindrical_frame;
    GeocentricCoord geocentric_frame;
    BarycentricCoord barycentric_frame;
public:
    Observatory() {};

    void set_cartesian(CartesianCoord);
    void set_cylindrical(CylindricalCoord);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    CartesianCoord get_cartesian();
    CylindricalCoord get_cylindric();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();

    Observatory(const Observatory& other);
    Observatory& operator=(const Observatory& other);
    Observatory(const Observatory&& other);
    Observatory& operator=(const Observatory&& other);
};