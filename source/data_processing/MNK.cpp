#include "MNK.h"



void MNK::calculate_dg_dx(ModelMeasure* condition)
{
    double dRA_dx[6] = { 0, 0, 0, 0, 0, 0 }; // dRA / dx, dRA / dy, dRA / dz, dRA / vx, dRA / vy, dRA / vz
    double dDEC_dx[6] = { 0, 0, 0, 0, 0, 0 }; // dDEC / dx, dDEC / dy, dDEC / dz, dDEC / vx, dDEC / vy, dDEC / vz

    /*
        RA = arctan(y/x)
        DEC = arctan (z / sqrt(x^2 + y^2))
    */

    double x = condition->get_barycentric().get_x();
    double y = condition->get_barycentric().get_y();
    double z = condition->get_barycentric().get_z();

    dRA_dx[0] = -1 * y / (help.POW_N(x, 2) + help.POW_N(y, 2));
    dRA_dx[1] = x / (help.POW_N(x, 2) + help.POW_N(y, 2));
    // dRA_dx[2] = 0, this dRA/dz
    // all dRA_dv = 0

    double a = help.POW_N(x, 2) + help.POW_N(y, 2) + help.POW_N(z, 2);
    double b = std::sqrt(help.POW_N(x, 2) + help.POW_N(y, 2));
    dDEC_dx[0] = -1 * x * z / (a * b);
    dDEC_dx[1] = -1 * y * z / (a * b);
    dDEC_dx[2] = (help.POW_N(x, 2) + help.POW_N(y, 2)) / (a * b);

    (*condition->get_dg_dx())[0][0] = dRA_dx[0];
    (*condition->get_dg_dx())[0][1] = dRA_dx[1];
    (*condition->get_dg_dx())[0][2] = dRA_dx[2];

    (*condition->get_dg_dx())[1][0] = dDEC_dx[0];
    (*condition->get_dg_dx())[1][1] = dDEC_dx[1];
    (*condition->get_dg_dx())[1][2] = dDEC_dx[2];
}



void MNK::calculate_dr_db(ModelMeasure* condition)
{
    condition->set_dr_db(-1 * (*condition->get_dg_dx()) * (*condition->get_dx_db()));
}

IntegrationVector MNK::Gauss_Newton(IntegrationVector x0, Matrix* A, Matrix* W, Matrix* R)
{
    IntegrationVector new_x0;



    return new_x0;
}
