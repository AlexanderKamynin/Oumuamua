#pragma once
#define EARTH_RADIUS 6378.140

#include "./measure/CartesianCoord.h"
#include "./measure/CylindricalCoord.h"


// ����� ��� �������� ����� ���������� ���������
class Converter
{
public:
	CartesianCoord cylindrical_to_cartesian(CylindricalCoord cylindrical_coord);
};

