#pragma once
#include <string>



class Date {
private:
    int year;
    int month;
    int day;

    int hours;
    int minutes;
    int seconds;

    double day_fraction;

    double JD;
    double MJD;
    double TT;
    double TDB;
    double TT_TDB;
public:
    Date() {};
    Date(std::string);

    friend bool operator< (const Date&, const Date&);
    friend bool operator> (const Date&, const Date&);
    friend bool operator== (const Date&, const Date&);

    void set_time_from_fraction();
    void set_time_from_string(std::string);

    int get_year();
    int get_month();
    int get_day();
    double get_day_fraction();
    double get_JD();
    double get_MJD();
    double get_TT();
    double get_TDB();
    double get_TT_TDB();

    void set_JD();
    void set_TT(double TT);
    void set_TT_TDB(double TT_TDB);
    void set_MJD(double MJD);
};