#pragma once
#include "Date.h"

#include <string>


/*
    The class whose objects store the interpolated time
*/
class InterpolationTime 
{
private:
    Date date;
    double TDB; // dinamic barycentric time
public:
    InterpolationTime() = default;

    //getters
    Date get_date();
    double get_TDB();

    //setters
    void set_date(Date);
    void set_TDB(std::string);
    void set_TDB(double);
};