#pragma once
#include "./measure/CelestianCoord.h"
#include "./measure/Date.h"

#include <string>

// класс для описания наблюдаемых величин
class Observation {
private:
    Date julian_date;// время исследования
    std::string code;// код обсерватории
    CelestialCoord ascension;// прямое восхождение
    CelestialCoord declination;// склонение
public:
    Observation() = default;
    // геттеры 
    Date get_julian_date();
    std::string get_code();
    CelestialCoord get_ascension();
    CelestialCoord get_declination();

    // сеттеры
    void set_julian_date(Date value);
    void set_code(std::string value);
    void set_ascension(CelestialCoord value);
    void set_declination(CelestialCoord value);

    void set_ascension_from_string(std::string value);
    void set_declination_from_string(std::string value);
};
