#pragma once


/*
	This class represent Earth rotation params
	@param x, y - centre of the orbit
	@param UT1-UTC - time, when x,y calculated
	@param MJD - time, when x,y calculated
*/
class EarthRotation
{
	double x = 0;
	double y = 0;
	double UT1_UTC = 0;
	double MJD = 0;
public:
	EarthRotation() = default;

	void set_x(double);
	void set_y(double);
	void set_UT1_UTC(double);
	void set_MJD(double);

	double get_MJD();
	double get_x();
	double get_y();
	double get_UT1_UTC();

	~EarthRotation() = default;
};

