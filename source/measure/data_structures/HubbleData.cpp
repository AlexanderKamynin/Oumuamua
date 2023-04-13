#include "HubbleData.h"


void HubbleData::set_date(Date date)
{
    this->date = date;
}

void HubbleData::set_geocentric(GeocentricCoord coord) 
{
    this->geocentric_position = coord;
}

void HubbleData::set_barycentric(BarycentricCoord coord)
{
    this->barycentric_position = coord;
}

Date HubbleData::get_date() 
{
    return this->date;
}

GeocentricCoord HubbleData::get_geocentric() 
{
    return this->geocentric_position;
}

BarycentricCoord HubbleData::get_barycentric()
{
    return this->barycentric_position;
}