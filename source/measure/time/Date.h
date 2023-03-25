#pragma once
#include <string>


/*
    This class is used to store the date in different formats
*/
class Date
{
private:
    int year;
    int month;
    int day;
    int hours;
    int minutes;
    int seconds;
    double day_fraction;

    double JD; // Julian date
    double MJD; // Modified Julian date
    double TT; // Terestial time
    double TDB; // Dinamic Barycentric time
public:
    Date() = default;

    Date(std::string);

    friend bool operator< (const Date&, const Date&);
    friend bool operator> (const Date&, const Date&);
    friend bool operator== (const Date&, const Date&);
    
    //getters
    int get_year();
    int get_month();
    int get_day();
    int get_hours();
    int get_minutes();
    int get_seconds();
    double get_day_fraction();
    double get_JD();
    double get_MJD();
    double get_TT();
    double get_TDB();

    //setters
    void set_time_from_fraction();
    void set_time_from_string(std::string);
    void set_MJD(double);
    void set_JD();
    void set_TT(double TT);
    void set_TDB(double TT_TDB);

    Date(const Date& other);
    Date& operator=(const Date& other);
    Date(const Date&& other);
    Date& operator=(const Date&& other);
};