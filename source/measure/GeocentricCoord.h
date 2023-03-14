#pragma once

class GeocentricCoord {
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    double get_x();
    double get_y();
    double get_z();
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);
};