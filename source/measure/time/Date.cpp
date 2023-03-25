#include "Date.h"


Date::Date(std::string date) 
{
    int prev = 0;
    int last = 0;
    for (int i = 0; i < 4; i++)
    {
        for (int j = prev; j < date.length() + 1; j++) 
        {
            if ((date[j] == ' ' and date[j + 1] != ' ') or (date[j] == '\0')) 
            {
                last = j;
                while (date[last - 1] == ' ') 
                {
                    last--;
                }
                switch (i) 
                {
                case 0:
                    year = std::stoi(date.substr(prev, last - prev));
                    break;
                case 1:
                    month = std::stoi(date.substr(prev, last - prev));
                    break;
                case 2:
                    day = std::stoi(date.substr(prev, last - prev));
                    day_fraction = std::stod(date.substr(prev, last - prev)) - day;
                    break;
                default:
                    break;
                }
                prev = j + 1;
                break;
            }

        }
    }
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

    this->JD = other.JD;
    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
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

    this->JD = other.JD;
    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
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

    this->JD = other.JD;
    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
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

    this->JD = other.JD;
    this->MJD = other.MJD;
    this->TT = other.TT;
    this->TDB = other.TDB;
    this->TDB = other.TDB;

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

void Date::set_time_from_fraction()
{
    double delta_t = day_fraction * 24;
    hours = int(delta_t);
    delta_t = (delta_t - hours) * 60;
    minutes = int(delta_t);
    delta_t = (delta_t - minutes) * 60;
    seconds = int(delta_t);
}

void Date::set_time_from_string(std::string time)
{
    int prev = 0;
    for (int i = 0; i < 3; i++) 
    {
        for (int j = prev; j < time.length() + 1; j++)
        {
            if ((time[j] == ' ') or (time[j] == '\0'))
            {
                switch (i) 
                {
                case 0:
                    year = std::stod(time.substr(prev, j - prev));
                    break;
                case 1:
                    month = std::stod(time.substr(prev, j - prev));
                    break;
                case 2:
                    day = std::stod(time.substr(prev, j - prev));
                    break;
                default:
                    break;
                }
                prev = j + 1;
                break;
            }
        }
    }
    prev = 10;
    for (int i = 0; i < 3; i++)
    {
        for (int j = prev; j < time.length() + 1; j++) 
        {
            if ((time[j] == ':') or (time[j] == '\0')) 
            {
                switch (i) 
                {
                case 0:
                    hours = std::stod(time.substr(prev, j - prev));
                    break;
                case 1:
                    minutes = std::stod(time.substr(prev, j - prev));
                    break;
                case 2:
                    seconds = std::stod(time.substr(prev, j - prev));
                    break;
                default:
                    break;
                }
                prev = j + 1;
                break;
            }
        }
    }
}

void Date::set_MJD(double MJD) 
{
    this->MJD = MJD;
}

void Date::set_JD() 
{
    // more about convert you can see here: https://ru.m.wikipedia.org/wiki/%D0%AE%D0%BB%D0%B8%D0%B0%D0%BD%D1%81%D0%BA%D0%B0%D1%8F_%D0%B4%D0%B0%D1%82%D0%B0
    double a = (14.0 - month) / 12.0;
    double y = year + 4800 - a;
    double m = month + 12 * a - 3;
    double JDN = day + (153.0 * m + 2) / 5.0 + 365 * y + y / 4.0 - y / 100.0 + y / 400.0 - 32045;
    JD = JDN + (hours - 12) / 24.0 + minutes / 1440.0 + seconds / 86400.0;
    MJD = JD - 2400000.5;
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

int Date::get_seconds()
{
    return this->seconds;
}

double Date::get_day_fraction() 
{
    return day_fraction;
}


double Date::get_JD()
{
    return JD;
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


void Date::set_TT(double TT)
{
    this->TT = TT;
}

void Date::set_TDB(double TT_TDB) 
{
    TDB = TT + TT_TDB;
}