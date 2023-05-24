#include "InterpolationTime.h"


Date TT_TDB_obj::get_date()
{
    return this->date;
}


double TT_TDB_obj::get_TT_TDB()
{
    return this->TT_TDB;
}


void TT_TDB_obj::set_date(Date date) 
{
    this->date = date;
}


void TT_TDB_obj::set_TT_TDB(std::string value) 
{
    this->TT_TDB = std::stod(value);
}


void TT_TDB_obj::set_TT_TDB(double value) 
{
    this->TT_TDB = value;
}