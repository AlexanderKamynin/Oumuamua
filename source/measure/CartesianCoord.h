#pragma once
#include <string>


class CartesianCoord {
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    //getters
    double get_x();
    double get_y();
    double get_z();

    //setters
    void set_from_string(std::string);
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);
};

