#include "BarycentricCoord.h"


double BarycentricCoord::get_alpha() 
{
    return this->alpha;
}


double BarycentricCoord::get_beta()
{
    return this->beta;
}


double BarycentricCoord::get_gamma() 
{
    return this->gamma;
}


void BarycentricCoord::set_alpha(double value) 
{
    this->alpha = value;
}


void BarycentricCoord::set_beta(double value) 
{
    this->beta = value;
}


void BarycentricCoord::set_gamma(double value)
{
    this->gamma = value;
}


void BarycentricCoord::increase(double value)
{
    this->alpha += value;
    this->beta += value;
    this->gamma += value;
}


void BarycentricCoord::multiply(double value)
{
    this->alpha *= value;
    this->beta *= value;
    this->gamma *= value;
}


double BarycentricCoord::length()
{
    return sqrt(help.POW_2(this->alpha) + help.POW_2(this->beta) + help.POW_2(this->gamma));
}


BarycentricCoord operator+(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.alpha = coordinate_1.alpha + coordinate_2.alpha;
    result.beta = coordinate_1.beta + coordinate_2.beta;
    result.gamma = coordinate_1.gamma + coordinate_2.gamma;
    return result;
}


BarycentricCoord operator-(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.alpha = coordinate_1.alpha - coordinate_2.alpha;
    result.beta = coordinate_1.beta - coordinate_2.beta;
    result.gamma = coordinate_1.gamma - coordinate_2.gamma;
    return result;
}


BarycentricCoord operator*(double value, BarycentricCoord coordinate_1)
{
    BarycentricCoord result;
    result.alpha = coordinate_1.alpha * value;
    result.beta = coordinate_1.beta * value;
    result.gamma = coordinate_1.gamma * value;
    return result;
}


BarycentricCoord operator/(BarycentricCoord coordinate_1, BarycentricCoord coordinate_2)
{
    BarycentricCoord result;
    result.alpha = coordinate_1.alpha / coordinate_2.alpha;
    result.beta = coordinate_1.beta / coordinate_2.beta;
    result.gamma = coordinate_1.gamma / coordinate_2.gamma;
    return result;
}


BarycentricCoord operator/(BarycentricCoord coordinate_1, double value)
{
    BarycentricCoord result;
    result.alpha = coordinate_1.alpha / value;
    result.beta = coordinate_1.beta / value;
    result.gamma = coordinate_1.gamma / value;
    return result;
}
