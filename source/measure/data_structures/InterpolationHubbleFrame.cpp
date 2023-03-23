#include "InterpolationHubbleFrame.h"

void InterpolationHubbleFrame::set_date(Date date)
{
    this->date = date;
}

void InterpolationHubbleFrame::set_geocentric(GeocentricCoord frame) 
{
    this->position_data = frame;
}

void InterpolationHubbleFrame::set_barycentric(BarycentricCoord frame)
{
    this->barycentric_frame = frame;
}

Date InterpolationHubbleFrame::get_date() 
{
    return date;
}

GeocentricCoord InterpolationHubbleFrame::get_geocentric() 
{
    return position_data;
}

BarycentricCoord InterpolationHubbleFrame::get_barycentric()
{
    return barycentric_frame;
}