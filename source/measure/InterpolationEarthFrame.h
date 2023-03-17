#pragma once
#include "Date.h"
#include <string>


class InterpolationEarthFrame {
private:
    Date julian_date;
    double x = NULL;
    double y = NULL;
    double z = NULL;
public:
    Date get_julian_date();
    void set_julian_date(Date value);
    double get_x();
    double get_y();
    double get_z();
    void set_x(std::string value);
    void set_y(std::string value);
    void set_z(std::string value);
    bool is_empty(std::string value);
};