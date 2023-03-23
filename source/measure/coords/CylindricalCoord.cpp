#include "CylindricalCoord.h"


double CylindricalCoord::get_longitude() 
{
    return longitude;
}

double CylindricalCoord::get_cos()
{
    return cos;
}

double CylindricalCoord::get_sin() 
{
    return sin;
}


bool CylindricalCoord::string_is_empty(std::string str) 
{
    for (char c : str) 
    {
        if (c != ' ') 
        {
            return false;
        }
    }
    return true;
}

// Перевод считываемой строки в число
void CylindricalCoord::set_longitude_from_string(std::string value) 
{
    this->longitude = string_is_empty(value) ? 0 : stod(value);
}

void CylindricalCoord::set_cos_from_string(std::string value) 
{
    this->cos = string_is_empty(value) ? 0 : stod(value);
}

void CylindricalCoord::set_sin_from_string(std::string value)
{
    this->sin = string_is_empty(value) ? 0 : stod(value);
}