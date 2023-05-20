#include "Date.h"
#include <iostream>


Date::Date(std::string date)
{
    std::vector<double> dates_part;
    dates_part = help.split(date, ' ', '\0');
    this->year = int(dates_part[0]);
    this->month = int(dates_part[1]);
    this->day = int(dates_part[2]);
    this->day_fraction = dates_part[2] - day;

    this->set_UTC_from_day_fraction();

    // set MJD
    // more about convert you can see here: https://ru.m.wikipedia.org/wiki/%D0%AE%D0%BB%D0%B8%D0%B0%D0%BD%D1%81%D0%BA%D0%B0%D1%8F_%D0%B4%D0%B0%D1%82%D0%B0
    int a = (14.0 - month) / 12.0;
    int y = year + 4800 - a;
    int m = month + 12 * a - 3;
    int JDN = day + int((153.0 * m + 2) / 5.0) + 365 * y + int(y / 4.0) - int(y / 100.0) + int(y / 400.0) - 32045;
    double JD = JDN + (hours - 12) / 24.0 + minutes / 1440.0 + seconds / 86400.0;
    MJD = JD - 2400000.5;
}   


void Date::set_day_fraction(double df)
{
    this->day_fraction = df;
}


void Date::set_UTC_from_day_fraction()
{
    double delta_t = day_fraction * 24;
    hours = int(delta_t);
    delta_t = (delta_t - hours) * 60;
    minutes = int(delta_t);
    delta_t = (delta_t - minutes) * 60;
    seconds = delta_t;
}


void Date::set_UTC_from_string(std::string time)
{
    std::vector<double> times_part;
    times_part = help.split(time, ':', '\0');
    this->hours = int(times_part[0]);
    this->minutes = int(times_part[1]);
    this->seconds = int(times_part[2]);
}


void Date::set_MJD(double MJD)
{
    this->MJD = MJD;
}


int Date::get_year()
{
    return year;
}

int Date::get_month()
{
    return month;
}

int Date::get_day()
{
    return day;
}

int Date::get_hours()
{
    return this->hours;
}


int Date::get_minutes()
{
    return this->minutes;
}

double Date::get_seconds()
{
    return this->seconds;
}

double Date::get_day_fraction()
{
    return day_fraction;
}


double Date::get_MJD()
{
    return MJD;
}

double Date::get_TT()
{
    return TT;
}


double Date::get_TDB()
{
    return TDB;
}

void Date::set_year(int year)
{
    this->year = year;
}

void Date::set_month(int month)
{
    this->month = month;
}

void Date::set_day(int day)
{
    this->day = day;
}

void Date::set_hours(int h)
{
    this->hours = h;
}

void Date::set_minutes(int m)
{
    this->minutes = m;
}

void Date::set_seconds(double s)
{
    this->seconds = s;
}


void Date::set_TT(double TT)
{
    this->TT = TT;
}


void Date::set_TDB(double TDB)
{
    this->TDB = TDB;
}


Date::Date(const Date& other)
{

    this->year = other.year;
    this->month = other.month;
    this->day = other.day;

    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;

    this->day_fraction = other.day_fraction;

    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
}


Date& Date::operator=(const Date& other)
{
    this->year = other.year;
    this->month = other.month;
    this->day = other.day;

    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;

    this->day_fraction = other.day_fraction;

    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;

    return *this;
}


Date::Date(const Date&& other)
{
    this->year = other.year;
    this->month = other.month;
    this->day = other.day;

    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;

    this->day_fraction = other.day_fraction;

    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
}


Date& Date::operator=(const Date&& other)
{
    this->year = other.year;
    this->month = other.month;
    this->day = other.day;

    this->hours = other.hours;
    this->minutes = other.minutes;
    this->seconds = other.seconds;

    this->day_fraction = other.day_fraction;

    this->MJD = other.MJD;
    this->TT = other.TT;

    return *this;
}


bool operator<(const Date& date1, const Date& date2)
{
    return date1.MJD < date2.MJD;
}


bool operator>(const Date& date1, const Date& date2)
{
    return date1.MJD > date2.MJD;
}


bool operator==(const Date& date1, const Date& date2)
{
    return date1.MJD == date2.MJD;
}
