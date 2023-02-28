#pragma once


class CartesianCoord {
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    //геттеры
    double get_x();
    double get_y();
    double get_z();

    //сеттеры
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);
};

