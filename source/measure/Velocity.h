#pragma once


class Velocity
{
private:
    double vx;
    double vy;
    double vz;
public:
    Velocity() {};
    void set(double, double, double);

    void set_vx(double);
    void set_vy(double);
    void set_vz(double);

    double get_vx();
    double get_vy();
    double get_vz();

    void increase(double);
    void multiply(double);

    friend Velocity operator+(Velocity, Velocity);
};