#include "EarthRotation.h"

void EarthRotation::set_x(double other_x)
{
	this->x = other_x;
}

void EarthRotation::set_y(double other_y)
{
	this->y = other_y;
}

void EarthRotation::set_UT1_UTC(double other_UT1_UTC)
{
	this->UT1_UTC = other_UT1_UTC;
}

void EarthRotation::set_MJD(int other_MJD)
{
	this->MJD = other_MJD;
}


int EarthRotation::get_MJD()
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
