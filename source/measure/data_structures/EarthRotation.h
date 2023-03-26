#pragma once
class EarthRotation
{
	double x;
	double y;
	double UT1_UTC;
	int MJD;
public:
	void set_x(double other_x);
	void set_y(double other_y);
	void set_UT1_UTC(double other_UT1_UTC);
	void set_MJD(int other_MJD);

	int get_MJD();
	double get_x();
	double get_y();
	double get_UT1_UTC();
	
};

