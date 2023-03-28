#include "InterpolationTime.h"


Date InterpolationTime::get_date()
{
    return this->date;
}


double InterpolationTime::get_TT_TDB()
{
    return this->TT_TDB;
}


void InterpolationTime::set_date(Date date) 
{
    this->date = date;
}


void InterpolationTime::set_TT_TDB(std::string value) 
{
    this->TT_TDB = std::stod(value);
}


void InterpolationTime::set_TT_TDB(double value) 
{
    this->TT_TDB = value;
}