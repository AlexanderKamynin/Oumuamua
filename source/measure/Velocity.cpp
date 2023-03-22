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

Velocity operator+(Velocity v1, Velocity v2) {
    Velocity result;
    result.vx = v1.vx + v2.vx;
    result.vy = v1.vy + v2.vy;
    result.vz = v1.vz + v2.vz;
    return result;
}

Velocity::Velocity(const Velocity& other) {
    this->vx = other.vx;
    this->vy = other.vy;
    this->vz = other.vz;
}

Velocity& Velocity::operator=(const Velocity& other) {
    this->vx = other.vx;
    this->vy = other.vy;
    this->vz = other.vz;
    return *this;
}

Velocity::Velocity(const Velocity&& other) {
    this->vx = other.vx;
    this->vy = other.vy;
    this->vz = other.vz;
}

Velocity& Velocity::operator=(const Velocity&& other) {
    this->vx = other.vx;
    this->vy = other.vy;
    this->vz = other.vz;
    return *this;
}

Velocity operator-(Velocity frame_1, Velocity frame_2) {
    Velocity result;
    result.vx = frame_1.vx - frame_2.vx;
    result.vy = frame_1.vy - frame_2.vy;
    result.vz = frame_1.vz - frame_2.vz;
    return result;
}

Velocity operator*(const double& m, Velocity frame) {
    Velocity result;
    result.vx = m * frame.vx;
    result.vy = m * frame.vy;
    result.vz = m * frame.vz;
    return result;
}

Velocity operator/(Velocity frame_1, Velocity frame_2) {
    Velocity result;
    result.vx = frame_1.vx / frame_2.vx;
    result.vy = frame_1.vy / frame_2.vy;
    result.vz = frame_1.vz / frame_2.vz;
    return result;
}

Velocity operator/(Velocity frame, const double& m) {
    Velocity result;
    result.vx = frame.vx / m;
    result.vy = frame.vy / m;
    result.vz = frame.vz / m;
    return result;
}