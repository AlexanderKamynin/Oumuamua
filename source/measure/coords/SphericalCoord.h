#pragma once

//Класс для сферических координат (в радианах)
class SphericalCoord {
private:
    double longitude;
    double latitude;
public:
    void set_longitude(double);
    void set_latitude(double);
    double get_longitude();
    double get_latitude();
};