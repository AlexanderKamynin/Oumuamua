#include <string>


//Небесные координаты
class CelestialCoord {
private:
    double h = 0;
    double m = 0;
    double s = 0;
public:
    CelestialCoord() = default;
    CelestialCoord(double h, double m, double s);
    void set_from_string(std::string);

    //геттеры
    double get_h();
    double get_m();
    double get_s();
};