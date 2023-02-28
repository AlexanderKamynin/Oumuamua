#include "Date.h"


Date::Date(std::string date) {
    int prev = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = prev; j < date.length() + 1; j++) {
            if ((date[j] == ' ') or (date[j] == '\0')) {
                switch (i) {
                case 0:
                    year = std::stoi(date.substr(prev, j - prev));
                    break;
                case 1:
                    month = std::stoi(date.substr(prev, j - prev));
                    break;
                case 2:
                    day = std::stoi(date.substr(prev, j - prev));
                    day_fraction = std::stod(date.substr(prev, j - prev)) - day;
                    break;
                default:
                    break;
                }
                prev = j + 1;
                break;
            }
        }
    }

    //Вычисление часа, минут и секунд через долю дня
    double delta_t = day_fraction * 24;
    hours = int(delta_t);
    delta_t = (delta_t - hours) * 60;
    minutes = int(delta_t);
    delta_t = (delta_t - minutes) * 60;
    seconds = int(delta_t);

    //Юлианская дата
    int a = int((14.0 - month) / 12.0);
    double y = year + 4800 - a;
    double m = month + 12 * a - 3;
    double JDN = day + int((153 * m + 2) / 5) + 365 * year + int(y / 4) - 32083;
    JD = JDN + (hours - 12) / 24.0 + minutes / 1440.0 + seconds / 86400.0;
    MJD = JD - 2400000.5;
}

int Date::get_year() {
    return year;
}

int Date::get_month() {
    return month;
}

int Date::get_day() {
    return day;
}

double Date::get_day_fraction() {
    return day_fraction;
}

double Date::get_JD() {
    return JD;
}

double Date::get_MJD() {
    return MJD;
}

double Date::get_TT() {
    return TT;
}

double Date::get_TBD() {
    return TBD;
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

void Date::set_day_fraction(double day_fraction)
{
    this->day_fraction = day_fraction;
}

void Date::set_JD(double JD) {
    this->JD = JD;
}

void Date::set_MJD(double MJD)
{
    this->MJD = MJD;
}

void Date::set_TT(double TT) {
    this->TT = TT;
}

void Date::set_TBD(double TBD) {
    this->TBD = TBD;
}