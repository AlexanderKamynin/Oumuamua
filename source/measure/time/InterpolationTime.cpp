#include "InterpolationTime.h"


Date InterpolationTime::get_date()
{
    return this->date;
}


double InterpolationTime::get_TDB()
{
    return this->TDB;
}


void InterpolationTime::set_date(Date date) 
{
    this->date = date;
}


void InterpolationTime::set_TDB(std::string value) 
{
    this->TDB = std::stod(value);
}