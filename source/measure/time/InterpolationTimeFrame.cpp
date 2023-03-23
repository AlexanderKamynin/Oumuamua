#include "InterpolationTimeFrame.h"


Date InterpolationTimeFrame::get_julian_date()
{
    return julian_date;
}

double InterpolationTimeFrame::get_TT_TDB()
{
    return TT_TDB;
};

void InterpolationTimeFrame::set_julian_date(Date value) 
{
    this->julian_date = value;
}

void InterpolationTimeFrame::set_TT_TDB(std::string value) 
{
    this->TT_TDB = is_empty(value) ? stod(value) : NULL;
}

bool InterpolationTimeFrame::is_empty(std::string value)
{
    for (char c : value) 
    {
        if (c != ' ') 
        {
            return true;
        }
    }
    return false;
}