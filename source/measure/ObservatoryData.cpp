#include "ObservatoryData.h"


void ObservatoryData::set_cartesian(CartesianCoord frame) {
    cartesian_frame = frame;
}

void ObservatoryData::set_cylindrical(CylindricalCoord frame) {
    cylindrical_frame = frame;
}

void ObservatoryData::set_geocentric(GeocentricCoord frame) {
    geocentric_frame = frame;
}

void ObservatoryData::set_barycentric(BarycentricCoord frame) {
    barycentric_frame = frame;
}

CartesianCoord ObservatoryData::get_cartesian() {
    return cartesian_frame;
}

CylindricalCoord ObservatoryData::get_cylindric() {
    return cylindrical_frame;
}

GeocentricCoord ObservatoryData::get_geocentric() {
    return geocentric_frame;
}

BarycentricCoord ObservatoryData::get_barycentric() {
    return barycentric_frame;
}
