#include "GeocentricCoord.h"


double GeocentricCoord::get_x()
{
    return this->x;
}


double GeocentricCoord::get_y()
{
    return this->y;
}


double GeocentricCoord::get_z()
{
    return this->z;
}


//void GeocentricCoord::set_from_string(std::string coord)
//{
//    int prev = 0;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = prev; j < coord.length()+1; j++)
//        {
//            if ((coord[j] == ' ') or (coord[j] == '\0'))
//            {
//                switch (i)
//                {
//                    case 0:
//                        x = std::stod(coord.substr(prev, j-prev));
//                        break;
//                    case 1:
//                        y = std::stod(coord.substr(prev, j-prev));
//                        break;
//                    case 2:
//                        z = std::stod(coord.substr(prev, j-prev));
//                        break;
//                    default:
//                        break;
//                }
//                prev = j+1;
//                break;
//            }
//        }
//    }   
//}

void GeocentricCoord::set_from_string(std::string coordinates)
{
    std::vector<double> vector_of_coords;
    vector_of_coords = help.split(coordinates, ' ', '\0');
    this->x = vector_of_coords[0];
    this->y = vector_of_coords[1];
    this->z = vector_of_coords[2];
}


void GeocentricCoord::set_x(double value)
{
    this->x = value;
}


void GeocentricCoord::set_y(double value)
{
    this->y = value;
}


void GeocentricCoord::set_z(double value)
{
    this->z = value;
}


GeocentricCoord::GeocentricCoord(const GeocentricCoord& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}


GeocentricCoord& GeocentricCoord::operator=(const GeocentricCoord& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}


GeocentricCoord::GeocentricCoord(const GeocentricCoord&& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}


GeocentricCoord& GeocentricCoord::operator=(const GeocentricCoord&& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}