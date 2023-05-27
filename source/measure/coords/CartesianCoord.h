#pragma once
#include <string>
#include <iostream>


/*
    This class stores Cartesian coordinates,
    has methods for initializing and getting data.
    Used to store coordinates and convert
*/
class CartesianCoord
{
private:
    double x = 0;
    double y = 0;
    double z = 0;
public:
    CartesianCoord() = default;

    //getters
    double get_x();
    double get_y();
    double get_z();

    //setters
    void set_x(double);
    void set_y(double);
    void set_z(double);

    void print();

    ~CartesianCoord() = default;
};