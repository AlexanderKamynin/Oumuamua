#pragma once
#include "../coords/BarycentricCoord.h"
#include "../coords/SphericalCoord.h"
#include "../../data_processing/Matrix/Matrix.h"
#include "../movement/Velocity.h"
#include "../time/Date.h"


class ModelMeasure
{
private:
    BarycentricCoord barycentric_position;
    SphericalCoord spherical_position;
    Velocity velocity;
    Date date;

    Matrix dx_db = Matrix(6, 6);
    Matrix dg_dx = Matrix(2, 6);
    Matrix dr_db = Matrix(2, 6); // -1 * dg(...)/dx * dx / db = M2x6
public:
    ModelMeasure() = default;


    //setters
    void set_barycentric(double, double, double);
    void set_barycentric(BarycentricCoord);
    void set_spherical(SphericalCoord);
    void set_velocity(double, double, double);
    void set_velocity(Velocity);
    void set_date(Date);
    void set_dx_db(Matrix);
    void set_dr_db(Matrix);


    //getters
    BarycentricCoord get_barycentric();
    SphericalCoord get_spherical();
    Velocity get_velocity();
    Date get_date();
    Matrix* get_dx_db();
    Matrix* get_dr_db();
    Matrix* get_dg_dx();
};

