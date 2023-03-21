#include "Velocity.h"

void Velocity::set(double vx, double vy, double vz) {
    this->vx = vx;
    this->vy = vy;
    this->vz = vz;
}

void Velocity::set_vx(double vx) {
    this->vx = vx;
}

void Velocity::set_vy(double vy) {
    this->vy = vy;
}

void Velocity::set_vz(double vz) {
    this->vx;
}

double Velocity::get_vx() {
    return vx;
}

double Velocity::get_vy() {
    return vy;
}

double Velocity::get_vz() {
    return vz;
}

void Velocity::increase(double delta) {
    vx += delta;
    vy += delta;
    vz += delta;
}

void Velocity::multiply(double delta) {
    vx *= delta;
    vy *= delta;
    vz *= delta;
}

Velocity operator+(Velocity v1, Velocity v2) 
{
    Velocity result;
    result.vx = v1.vx + v2.vx;
    result.vy = v1.vy + v2.vy;
    result.vz = v1.vz + v2.vz;
    return result;
}

