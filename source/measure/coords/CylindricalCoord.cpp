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
    bool flage = true;
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] == ' ')
        {
            flage = false;
            break;
        }
    }
    if (flage)
    {
        this->longitude = stod(value);
    }
}


void CylindricalCoord::set_cos_from_string(std::string value)
{
    bool flage = true;
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] == ' ')
        {
            flage = false;
            break;
        }
    }
    if (flage)
    {
        this->cos = stod(value);
    }
}


void CylindricalCoord::set_sin_from_string(std::string value)
{
    bool flage = true;
    for (int i = 0; i < value.size(); i++)
    {
        if (value[i] == ' ')
        {
            flage = false;
            break;
        }
    }
    if (flage)
    {
        this->sin = stod(value);
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
