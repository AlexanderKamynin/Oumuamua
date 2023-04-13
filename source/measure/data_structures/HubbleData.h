#pragma once
#include "../coords/GeocentricCoord.h"
#include "../coords/BarycentricCoord.h"

#include "../time/Date.h"


/*
    This class describes Hubble's position and time, for interpolation
*/
class HubbleData 
{
private:
    Date date;
    GeocentricCoord geocentric_position;
    BarycentricCoord barycentric_position;
public:
    HubbleData() = default;

    //setters
    void set_date(Date);
    void set_geocentric(GeocentricCoord);
    void set_barycentric(BarycentricCoord);

    //getters
    Date get_date();
    GeocentricCoord get_geocentric();
    BarycentricCoord get_barycentric();
};