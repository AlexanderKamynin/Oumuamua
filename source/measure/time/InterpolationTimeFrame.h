#pragma once
#include "Date.h"

#include <string>

// ласс представл€ющий интерполированное врем€ в шкале TDB
class InterpolationTimeFrame {
private:
    Date julian_date;
    double TT_TDB;
public:
    Date get_julian_date();
    double get_TT_TDB();
    void set_julian_date(Date value);
    void set_TT_TDB(std::string value);
    bool is_empty(std::string value);
};