#include "EarthRotation.h"

void EarthRotation::set_x(double x)
{
	this->x = x;
}

void EarthRotation::set_y(double y)
{
	this->y = y;
}

void EarthRotation::set_UT1_UTC(double UT1_UTC)
{
	this->UT1_UTC = UT1_UTC;
}

void EarthRotation::set_MJD(double MJD)
{
	this->MJD = MJD;
}


double EarthRotation::get_MJD()
{
	return this->MJD;
}

double EarthRotation::get_x()
{
	return this->x;
}

double EarthRotation::get_y()
{
	return this->y;
}

double EarthRotation::get_UT1_UTC()
{
	return this->UT1_UTC;
}
