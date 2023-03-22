#pragma once
#include <string>

//Класс геоцентрических координат
class GeocentricCoord {
private:
    double x = NULL;
    double y = NULL;
    double z = NULL;
public:
    GeocentricCoord() {};
    void set_from_string(std::string);
    double get_x();
    double get_y();
    double get_z();
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);
    GeocentricCoord(const GeocentricCoord& other);
    GeocentricCoord& operator=(const GeocentricCoord& other);
    GeocentricCoord(const GeocentricCoord&& other);
    GeocentricCoord& operator=(const GeocentricCoord&& other);
};