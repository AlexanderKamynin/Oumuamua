#include "Converter.h"

CartesianCoord Converter::cylindrical_to_cartesian(CylindricalCoord cylindrical_coord)
{
    CartesianCoord cartesian_coord;
    if (cylindrical_coord.get_longitude() != 0) {
        if (cylindrical_coord.get_cos() != 0) {
            cartesian_coord.set_x(cos(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
            cartesian_coord.set_y(sin(cylindrical_coord.get_longitude()) * cylindrical_coord.get_cos() * EARTH_RADIUS);
        }
        if (cylindrical_coord.get_sin() != 0)
            cartesian_coord.set_z(cylindrical_coord.get_sin() * EARTH_RADIUS);
    }
    return cartesian_coord;
}
