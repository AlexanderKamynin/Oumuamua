#include "InterpolationEarthFrame.h"


Date InterpolationEarthFrame::get_julian_date() {
    return julian_date;
}

double InterpolationEarthFrame::get_x() {
    return x;
}

double InterpolationEarthFrame::get_y() {
    return y;
}

double InterpolationEarthFrame::get_z() {
    return z;
}

void InterpolationEarthFrame::set_julian_date(Date value) {
    this->julian_date = value;
}

void InterpolationEarthFrame::set_x(std::string value) {
    x = is_empty(value) ? stod(value) : NULL;
}

void InterpolationEarthFrame::set_y(std::string  value) {
    y = is_empty(value) ? stod(value) : NULL;
}

void InterpolationEarthFrame::set_z(std::string  value) {
    z = is_empty(value) ? stod(value) : NULL;
}

bool InterpolationEarthFrame::is_empty(std::string value) {
    for (char c : value) {
        if (c != ' ') {
            return true;
        }
    }
    return false;
}
