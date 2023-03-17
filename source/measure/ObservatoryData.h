#pragma once
#include "CartesianCoord.h"
#include "CylindricalCoord.h"
#include "GeocentricCoord.h"
#include "BarycentricCoord.h"


class ObservatoryData {
private:
    CartesianCoord cartesian_frame;
    CylindricalCoord cylindrical_frame;
    GeocentricCoord geocentric_frame;
    BarycentricCoord barycentric_frame;
public:
    void set_cartesian(CartesianCoord);
    void set_cylindrical(CylindricalCoord);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    CartesianCoord get_cartesian();
    CylindricalCoord get_cylindric();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};