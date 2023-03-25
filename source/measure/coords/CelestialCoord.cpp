#include "CelestialCoord.h"


//Получение координат из строки
void CelestialCoord::set_from_string(std::string string)
{
    std::vector<double> vector_of_coords;
    vector_of_coords = help.split(string, ' ', '\0');
    this->hours = vector_of_coords[0];
    this->minutes = vector_of_coords[1];
    this->seconds = vector_of_coords[2];
}


double CelestialCoord::get_hours()
{
    return this->hours;
}


double CelestialCoord::get_minutes()
{
    return this->minutes;
}


double CelestialCoord::get_seconds()
{
    return this->seconds;
}


CelestialCoord::CelestialCoord(const CelestialCoord& other)
{
    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;
}


CelestialCoord& CelestialCoord::operator=(const CelestialCoord& other)
{
    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;
    return *this;
}


CelestialCoord::CelestialCoord(const CelestialCoord&& other)
{
    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;
}


CelestialCoord& CelestialCoord::operator=(const CelestialCoord&& other)
{
    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;
    return *this;
}
