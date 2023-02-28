#include "CelestianCoord.h"


CelestialCoord::CelestialCoord(double h, double m, double s) {
    this->h = h;
    this->m = m;
    this->s = s;
}


//Получение координат из строки
void CelestialCoord::set_from_string(std::string params) {
    char param;
    int prev = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = prev; j < params.length() + 1; j++) {
            if ((params[j] == ' ') or (params[j] == '\0')) {
                switch (i) {
                case 0:
                    h = std::stod(params.substr(prev, j - prev));
                    break;
                case 1:
                    m = std::stod(params.substr(prev, j - prev));
                    break;
                case 2:
                    s = std::stod(params.substr(prev, j - prev));
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

double CelestialCoord::get_h() {
    return this->h;
}

double CelestialCoord::get_m() {
    return this->m;
}

double CelestialCoord::get_s() {
    return this->s;
}