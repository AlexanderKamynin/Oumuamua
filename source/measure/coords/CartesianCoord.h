#pragma once
#include <string>
#include "../../Helpers.h"


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
    Helpers help;
public:
    CartesianCoord() = default;

    //getters
    double get_x();
    double get_y();
    double get_z();

    //setters
    void set_x(double value);
    void set_y(double value);
    void set_z(double value);

    //operators
    CartesianCoord(const CartesianCoord& other);
    CartesianCoord& operator=(const CartesianCoord& other);
    CartesianCoord(const CartesianCoord&& other);
    CartesianCoord& operator=(const CartesianCoord&& other);
};