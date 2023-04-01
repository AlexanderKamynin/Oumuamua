#include "SphericalCoord.h"


void SphericalCoord::set_right_ascension(double right_ascension)
{
    this->right_ascension = right_ascension;
}


void SphericalCoord::set_declination(double declination) 
{
    this->declination = declination;
}


void SphericalCoord::set_RA_in_hours_system_from_string(std::string string)
{
    std::vector<double> vector_of_coords;
    vector_of_coords = help.split(string, ' ', '\0');
    this->RA_in_hours_system[0] = vector_of_coords[0];
    this->RA_in_hours_system[1] = vector_of_coords[1];
    this->RA_in_hours_system[2] = vector_of_coords[2];
}


void SphericalCoord::set_DEC_in_hours_system_from_string(std::string string)
{
    std::vector<double> vector_of_coords;
    vector_of_coords = help.split(string, ' ', '\0');
    this->DEC_in_hours_system[0] = vector_of_coords[0];
    this->DEC_in_hours_system[1] = vector_of_coords[1];
    this->DEC_in_hours_system[2] = vector_of_coords[2];
}


double SphericalCoord::get_right_ascension()
{
    return right_ascension;
}


double SphericalCoord::get_declination()
{
    return declination;
}

double* SphericalCoord::get_RA_in_hours_system()
{
    return this->RA_in_hours_system;
}

double* SphericalCoord::get_DEC_in_hours_system()
{
    return this->DEC_in_hours_system;
}
