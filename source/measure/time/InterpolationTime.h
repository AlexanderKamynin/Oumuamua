#pragma once
#include "Date.h"

#include <string>


/*
    The class whose objects store the interpolated time
*/
class TT_TDB_obj 
{
private:
    Date date;
    double TT_TDB; // dynamical barycentric time
public:
    TT_TDB_obj() = default;

    //getters
    Date get_date();
    double get_TT_TDB();

    //setters
    void set_date(Date);
    void set_TT_TDB(std::string);
    void set_TT_TDB(double);
};