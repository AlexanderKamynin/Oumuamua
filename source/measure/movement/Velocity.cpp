#include "Velocity.h"


double Velocity::get_vx() 
{
    return this->vx;
}


double Velocity::get_vy() 
{
    return this->vy;
}


double Velocity::get_vz() 
{
    return this->vz;
}


void Velocity::set_all_velocity(double vx, double vy, double vz)
{
    this->vx = vx;
    this->vy = vy;
    this->vz = vz;
}


void Velocity::set_vx(double vx)
{
    this->vx = vx;
}


void Velocity::set_vy(double vy)
{
    this->vy = vy;
}


void Velocity::set_vz(double vz)
{
    this->vz = vz;
}


void Velocity::increase(double value) 
{
    this->vx += value;
    this->vy += value;
    this->vz += value;
}


void Velocity::multiply(double value)
{
    this->vx *= value;
    this->vy *= value;
    this->vz *= value;
}

void Velocity::print()
{
    std::cout << "vx= " << this->vx / 86400 << " vy= " << this->vy / 86400 << " vz= " << this->vz / 86400 << " in km/s\n";
}


Velocity operator+(Velocity velocity1, Velocity velocity2) 
{
    Velocity result;
    result.vx = velocity1.vx + velocity2.vx;
    result.vy = velocity1.vy + velocity2.vy;
    result.vz = velocity1.vz + velocity2.vz;
    return result;
}


Velocity operator-(Velocity velocity1, Velocity velocity2)
{
    Velocity result;
    result.vx = velocity1.vx - velocity2.vx;
    result.vy = velocity1.vy - velocity2.vy;
    result.vz = velocity1.vz - velocity2.vz;
    return result;
}


Velocity operator*(Velocity velocity1, double value)
{
    Velocity result;
    result.vx = velocity1.vx * value;
    result.vy = velocity1.vy * value;
    result.vz = velocity1.vz * value;
    return result;
}


Velocity operator/(Velocity velocity1, Velocity velocity2)
{
    Velocity result;
    result.vx = velocity1.vx / velocity2.vx;
    result.vy = velocity1.vy / velocity2.vy;
    result.vz = velocity1.vz / velocity2.vz;
    return result;
}


Velocity operator/(Velocity velocity1, double value)
{
    Velocity result;
    result.vx = velocity1.vx / value;
    result.vy = velocity1.vy / value;
    result.vz = velocity1.vz / value;
    return result;
}