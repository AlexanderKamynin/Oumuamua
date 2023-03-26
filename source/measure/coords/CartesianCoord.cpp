#include "CartesianCoord.h"


double CartesianCoord::get_x()
{
    return this->x;
}

double CartesianCoord::get_y()
{
    return this->y;
}

double CartesianCoord::get_z()
{
    return this->z;
}


//void CartesianCoord::set_from_string(std::string coordinates)
//{
//    int prev = 0;
//    for (int i = 0; i < 4; i++)
//    {
//        for (int j = prev; j < coordinates.length()+1; j++)
//        {
//            if ((coordinates[j] == ' ') or (coordinates[j] == '\0'))
//            {
//                switch (i)
//                {
//                    case 0:
//                        x = std::stod(coordinates.substr(prev, j-prev));
//                        break;
//                    case 1:
//                        y = std::stod(coordinates.substr(prev, j-prev));
//                        break;
//                    case 2:
//                        z = std::stod(coordinates.substr(prev, j-prev));
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


void CartesianCoord::set_x(double value)
{
    this->x = value;
}


void CartesianCoord::set_y(double value)
{
    this->y = value;
}


void CartesianCoord::set_z(double value)
{
    this->z = value;
}


//operators
CartesianCoord::CartesianCoord(const CartesianCoord& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

CartesianCoord& CartesianCoord::operator=(const CartesianCoord& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}

CartesianCoord::CartesianCoord(const CartesianCoord&& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;
}

CartesianCoord& CartesianCoord::operator=(const CartesianCoord&& other)
{
    this->x = other.x;
    this->y = other.y;
    this->z = other.z;

    return *this;
}