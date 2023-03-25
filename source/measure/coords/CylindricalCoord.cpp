#include "CylindricalCoord.h"


double CylindricalCoord::get_longitude()
{
    return this->longitude;
}


double CylindricalCoord::get_cos()
{
    return this->cos;
}


double CylindricalCoord::get_sin()
{
    return this->sin;
}


void CylindricalCoord::set_longitude_from_string(std::string value)
{
    std::string longitude = "";
    for (int i = 0; i < value.size(); i++) 
    {
        if (value[i] != ' ')
        {
            longitude += value[i];
        }
    }
    if (longitude != "")
    {
        this->longitude = std::stod(longitude);
    }
}


void CylindricalCoord::set_cos_from_string(std::string value)
{
    std::string cos = "";
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] != ' ')
        {
            cos += value[i];
        }
    }
    if (cos != "")
    {
        this->cos = std::stod(cos);
    }
}


void CylindricalCoord::set_sin_from_string(std::string value)
{
    std::string sin = "";
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] != ' ')
        {
            sin += value[i];
        }
    }
    if (sin != "")
    {
        this->sin = std::stod(sin);
    }
}



CylindricalCoord::CylindricalCoord(const CylindricalCoord& other)
{
    this->longitude = other.longitude;
    this->cos = other.cos;
    this->sin = other.sin;
}


CylindricalCoord& CylindricalCoord::operator=(const CylindricalCoord& other)
{
    this->longitude = other.longitude;
    this->cos = other.cos;
    this->sin = other.sin;

    return *this;
}


CylindricalCoord::CylindricalCoord(const CylindricalCoord&& other)
{
    this->longitude = other.longitude;
    this->cos = other.cos;
    this->sin = other.sin;
}


CylindricalCoord& CylindricalCoord::operator=(const CylindricalCoord&& other)
{
    this->longitude = other.longitude;
    this->cos = other.cos;
    this->sin = other.sin;

    return *this;
}
