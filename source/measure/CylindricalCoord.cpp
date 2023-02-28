#include "CylindricalCoord.h"


double CylindricalCoord::get_longitude() {
    return longitude;
}

double CylindricalCoord::get_cos() {
    return cos;
}

double CylindricalCoord::get_sin() {
    return sin;
}


bool CylindricalCoord::is_empty(std::string value) {
    for (char c : value) {
        if (c != ' ') {
            return true;
        }
    }
    return false;
}

// Перевод считываемой строки в число
void CylindricalCoord::set_longitude_from_string(std::string value) {
    this->longitude = is_empty(value) ? stod(value) : 0;
}

void CylindricalCoord::set_cos_from_string(std::string value) {
    this->cos = is_empty(value) ? stod(value) : 0;
}

void CylindricalCoord::set_sin_from_string(std::string value) {
    this->sin = is_empty(value) ? stod(value) : 0;
}