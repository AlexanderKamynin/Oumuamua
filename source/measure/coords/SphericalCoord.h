#pragma once


/*
    This class is a coordinate system describing spherical coordinates on Earth. 
    The parameters are right ascension and declination.
*/
class SphericalCoord 
{
private:
    double longitude = 0;
    double latitude = 0;
public:
    SphericalCoord() = default;

    //setters
    void set_longitude(double);
    void set_latitude(double);

    //getters
    double get_longitude();
    double get_latitude();
};