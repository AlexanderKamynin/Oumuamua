#pragma once
#include <string>


// ласс даты в формате "год мес€ц день.дол€ дн€"
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
    double TBD;
public:
    Date() = default;
    Date(std::string);

    //геттеры
    int get_year();
    int get_month();
    int get_day();
    double get_day_fraction();
    double get_JD();
    double get_MJD();
    double get_TT();
    double get_TBD();

    //сеттеры
    void set_year(int year);
    void set_month(int month);
    void set_day(int day);
    void set_day_fraction(double day_fraction);
    void set_JD(double JD);
    void set_MJD(double MJD);
    void set_TT(double TT);
    void set_TBD(double TT);
};