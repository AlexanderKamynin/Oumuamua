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

    dRA_dx[0] = -1 * y / (x * x + y * y);
    dRA_dx[1] = x / (x * x + y * y);
    // dRA_dx[2] = 0, this dRA/dz
    // all dRA_dv = 0

    double a = x * x + y * y + z * z;
    double b = std::sqrt(x * x + y * y);
    dDEC_dx[0] = -1 * x * z / (a * b);
    dDEC_dx[1] = -1 * y * z / (a * b);
    dDEC_dx[2] = (x * x + y * y) / (a * b);

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
    
    Matrix gradient_f = ((*A).transpose()) * (*W) * (*A); // 6x6 matrix
    Matrix f_b = ((*A).transpose()) * (*W) * (*R); // f(^b) is vector 6x1
    Matrix solution_system = solve_system(&gradient_f, &f_b);

    IntegrationVector new_x0;
    new_x0.set_barycentric(x0.get_barycentric().get_x() - solution_system[0][0], x0.get_barycentric().get_y() - solution_system[1][0], x0.get_barycentric().get_z() - solution_system[2][0]);
    new_x0.set_velocity(x0.get_velocity().get_vx() - solution_system[3][0], x0.get_velocity().get_vy() - solution_system[4][0], x0.get_velocity().get_vz() - solution_system[5][0]);
    return new_x0;
}

Matrix MNK::solve_system(Matrix* gradient_f, Matrix* f_b)
{
    Matrix decomposed_matrix = f_b->Cholesky_decomposition(*gradient_f);

    Matrix y(decomposed_matrix.rows(), f_b->columns()); // find y

    for (int i = 0; i < decomposed_matrix.rows(); i++)
    {
        double sum = 0;
        for (int j = 0; j < i; j++) 
        {
            sum += decomposed_matrix[i][j] * y[j][0];
        }
        if (decomposed_matrix[i][i] == 0)
        {
            y[i][0] = 0;
        }
        else 
        {
            y[i][0] = ((*f_b)[i][0] - sum) / decomposed_matrix[i][i];
        }
    }


    decomposed_matrix = decomposed_matrix.transpose(); // find x0

    Matrix solution_system(decomposed_matrix.rows(), y.columns());


    for (int i = decomposed_matrix.rows() - 1; i > -1; i--)
    {
        double sum = 0;
        for (int j = i; j < decomposed_matrix.rows(); j++)
        {
            sum += decomposed_matrix[i][j] * solution_system[j][0];
        }
        if (decomposed_matrix[i][i] == 0) 
        {
            solution_system[i][0] = 0;
        }
        else 
        {
            solution_system[i][0] = (y[i][0] - sum) / decomposed_matrix[i][i];
        }
    }

    return solution_system;
}
