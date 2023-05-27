#pragma once
#include <iostream>


/*
    This class describes the speed of an object whose vector is decomposed into three components

    vx, xy, xz - decomposition of the vector into components x, y, z
*/
class Velocity 
{
private:
    double vx;
    double vy;
    double vz;
public:
    Velocity() = default;


    //getters
    double get_vx();
    double get_vy();
    double get_vz();


    //setters
    void set_vx(double);
    void set_vy(double);
    void set_vz(double);
    void set_all_velocity(double, double, double);


    /*
        Method for + and * on one value
    */
    void increase(double);
    void multiply(double);

    void print();


    /*
        Operation between Velocity objects
    */
    friend Velocity operator+(Velocity, Velocity);
    friend Velocity operator-(Velocity, Velocity);
    friend Velocity operator*(Velocity, double);
    friend Velocity operator/(Velocity, Velocity);
    friend Velocity operator/(Velocity, double);

    ~Velocity() = default;
};