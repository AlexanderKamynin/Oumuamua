#pragma once
#include "../coords/BarycentricCoord.h"
#include "../coords/SphericalCoord.h"
#include "../movement/Velocity.h"
#include "../time/Date.h"


class ModelMeasure
{
private:
    BarycentricCoord barycentric_position;
    SphericalCoord spherical_position;
    Velocity velocity;
    Date date;
public:
    ModelMeasure() = default;


    //setters
    void set_barycentric(double, double, double);
    void set_barycentric(BarycentricCoord);
    void set_spherical(SphericalCoord);
    void set_velocity(double, double, double);
    void set_velocity(Velocity);
    void set_date(Date);


    //getters
    BarycentricCoord get_barycentric();
    SphericalCoord get_spherical();
    Velocity get_velocity();
    Date get_date();
};

