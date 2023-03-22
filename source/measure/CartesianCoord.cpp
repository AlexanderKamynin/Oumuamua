#include "CartesianCoord.h"


double CartesianCoord::get_x() {
    return this->x;
}

double CartesianCoord::get_y() {
    return this->y;
}

double CartesianCoord::get_z() {
    return this->z;
}

void CartesianCoord::set_x(double value) {
    this->x = value;
}

void CartesianCoord::set_y(double value) {
    this->y = value;
}

void CartesianCoord::set_z(double value) {
    this->z = value;
}

void CartesianCoord::set_from_string(std::string coord) {
    /*
        read the data from string
    */
    int prev = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = prev; j < coord.length() + 1; j++) {
            if ((coord[j] == ' ') || (coord[j] == '\0')) {
                switch (i) {
                case 0:
                    this->x = std::stod(coord.substr(prev, j - prev));
                    break;
                case 1:
                    this->y = std::stod(coord.substr(prev, j - prev));
                    break;
                case 2:
                    this->z = std::stod(coord.substr(prev, j - prev));
                    break;
                default:
                    break;
                }
                prev = j + 1;
                break;
            }
        }
    }
}