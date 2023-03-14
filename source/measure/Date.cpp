#include "Date.h"


Date::Date(std::string date)
{
    int prev = 0;
    int last = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = prev; j < date.length() + 1; j++) {
            if ((date[j] == ' ' and date[j + 1] != ' ') or (date[j] == '\0')) {
                last = j;
                while (date[last - 1] == ' ') {
                    last--;
                }
                switch (i) {
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

bool operator<(const Date& date1, const Date& date2) {
    return date1.MJD < date2.MJD;
}

bool operator>(const Date& date1, const Date& date2) {
    return date1.MJD > date2.MJD;
}

bool operator==(const Date& date1, const Date& date2) {
    return date1.MJD == date2.MJD;
}

void Date::set_time_from_fraction() {
    double delta_t = day_fraction * 24;
    hours = int(delta_t);
    delta_t = (delta_t - hours) * 60;
    minutes = int(delta_t);
    delta_t = (delta_t - minutes) * 60;
    seconds = int(delta_t);
}

void Date::set_time_from_string(std::string time) {
    //
}

void Date::set_JD() {
    // more about convert you can see here: https://ru.m.wikipedia.org/wiki/%D0%AE%D0%BB%D0%B8%D0%B0%D0%BD%D1%81%D0%BA%D0%B0%D1%8F_%D0%B4%D0%B0%D1%82%D0%B0
    int a = int((14.0 - month) / 12.0);
    double y = year + 4800 - a;
    double m = month + 12 * a - 3;
    double JDN = day + int((153 * m + 2) / 5) + 365 * year + int(y / 4) - 32083;
    JD = JDN + (hours - 12) / 24.0 + minutes / 1440.0 + seconds / 86400.0;
    MJD = JD - 2400000.5;
}

int Date::get_year() {
    return year;
};

int Date::get_month() {
    return month;
};

int Date::get_day() {
    return day;
};

double Date::get_day_fraction() {
    return day_fraction;
};

double Date::get_JD() {
    return JD;
};

double Date::get_MJD() {
    return MJD;
};

double Date::get_TT() {
    return TT;
};

double Date::get_TDB() {
    return TDB;
};

double Date::get_TT_TDB() {
    return TT_TDB;
};

void Date::set_TT(double TT) {
    this->TT = TT;
};

void Date::set_TT_TDB(double TT_TDB) {
    this->TT_TDB = TT_TDB;
    TDB = TT + TT_TDB;
};