#include "BarycentricCoord.h"


double BarycentricCoord::get_x() 
{
    return x;
}

double BarycentricCoord::get_y()
{
    return y;
}

double BarycentricCoord::get_z() 
{
    return z;
}

void BarycentricCoord::set_x(double value) 
{
    x = value;
}

void BarycentricCoord::set_y(double value) 
{
    y = value;
}

void BarycentricCoord::set_z(double value)
{
    z = value;
}

void BarycentricCoord::increase(double delta)
{
    x += delta;
    y += delta;
    z += delta;
}

void BarycentricCoord::multiply(double delta)
{
    x *= delta;
    y *= delta;
    z *= delta;
}

double BarycentricCoord::len() {
    return sqrt(x * x + y * y + z * z);
}

BarycentricCoord operator+(BarycentricCoord frame_1, BarycentricCoord frame_2) {
    BarycentricCoord result;
    result.x = frame_1.x + frame_2.x;
    result.y = frame_1.y + frame_2.y;
    result.z = frame_1.z + frame_2.z;
    return result;
}

BarycentricCoord operator-(BarycentricCoord frame_1, BarycentricCoord frame_2) {
    BarycentricCoord result;
    result.x = frame_1.x - frame_2.x;
    result.y = frame_1.y - frame_2.y;
    result.z = frame_1.z - frame_2.z;
    return result;
}

BarycentricCoord operator*(const double& m, BarycentricCoord frame) {
    BarycentricCoord result;
    result.x = m * frame.x;
    result.y = m * frame.y;
    result.z = m * frame.z;
    return result;
}

BarycentricCoord operator/(BarycentricCoord frame_1, BarycentricCoord frame_2) {
    BarycentricCoord result;
    result.x = frame_1.x / frame_2.x;
    result.y = frame_1.y / frame_2.y;
    result.z = frame_1.z / frame_2.z;
    return result;
}

BarycentricCoord operator/(BarycentricCoord frame, const double& m) {
    BarycentricCoord result;
    result.x = frame.x / m;
    result.y = frame.y / m;
    result.z = frame.z / m;
    return result;
}
