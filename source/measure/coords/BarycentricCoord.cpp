#include "BarycentricCoord.h"


double BarycentricCoord::get_x() 
{
    return this->x;
}


double BarycentricCoord::get_y()
{
    return this->y;
}


double BarycentricCoord::get_z() 
{
    return this->z;
}


void BarycentricCoord::set_x(double value) 
{
    this->x = value;
}


void BarycentricCoord::set_y(double value) 
{
    this->y = value;
}


void BarycentricCoord::set_z(double value)
{
    this->z = value;
}


void BarycentricCoord::increase(double value)
{
    this->x += value;
    this->y += value;
    this->z += value;
}


void BarycentricCoord::multiply(double value)
{
    this->x *= value;
    this->y *= value;
    this->z *= value;
}


double BarycentricCoord::length()
{
    return sqrt(help.POW_2(this->x) + help.POW_2(this->y) + help.POW_2(this->z));
}


BarycentricCoord operator+(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.x = coordinate_1.x + coordinate_2.x;
    result.y = coordinate_1.y + coordinate_2.y;
    result.z = coordinate_1.z + coordinate_2.z;
    return result;
}


BarycentricCoord operator-(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.x = coordinate_1.x - coordinate_2.x;
    result.y = coordinate_1.y - coordinate_2.y;
    result.z = coordinate_1.z - coordinate_2.z;
    return result;
}


BarycentricCoord operator*(BarycentricCoord coordinate_1, double value)
{
    BarycentricCoord result;
    result.x = coordinate_1.x * value;
    result.y = coordinate_1.y * value;
    result.z = coordinate_1.z * value;
    return result;
}

BarycentricCoord operator*(double value, BarycentricCoord coordinate_1)
{
    BarycentricCoord result;
    result.x = coordinate_1.x * value;
    result.y = coordinate_1.y * value;
    result.z = coordinate_1.z * value;
    return result;
}


BarycentricCoord operator/(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.x = coordinate_1.x / coordinate_2.x;
    result.y = coordinate_1.y / coordinate_2.y;
    result.z = coordinate_1.z / coordinate_2.z;
    return result;
}


BarycentricCoord operator/(BarycentricCoord coordinate_1, double value)
{
    BarycentricCoord result;
    result.x = coordinate_1.x / value;
    result.y = coordinate_1.y / value;
    result.z = coordinate_1.z / value;
    return result;
}
