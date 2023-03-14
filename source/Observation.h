#pragma once
#include "./measure/CelestianCoord.h"
#include "./measure/Date.h"

#include <string>

// ����� ��� �������� ����������� �������
class Observation {
private:
    Date julian_date;// ����� ������������
    std::string code;// ��� ������������
    CelestianCoord ascension;// ������ �����������
    CelestianCoord declination;// ���������
public:
    Observation() = default;
    // ������� 
    Date get_julian_date();
    std::string get_code();
    CelestianCoord get_ascension();
    CelestianCoord get_declination();

    // �������
    void set_julian_date(Date value);
    void set_code(std::string value);
    void set_ascension(CelestianCoord value);
    void set_declination(CelestianCoord value);

    void set_ascension_from_string(std::string value);
    void set_declination_from_string(std::string value);
};
