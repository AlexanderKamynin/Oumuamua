#pragma once


class BarycentricCoord
{
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    BarycentricCoord() = default;
    double get_x();
    double get_y();
    double get_z();
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);


    void increase(double);
    void multiply(double);

    friend BarycentricCoord operator+(BarycentricCoord, BarycentricCoord);
};
