#include "Integration.h"


IntegrationVector Integration::derivate_function(IntegrationVector current_condition, std::map<std::string, std::vector<IntegrationVector>>* planets)
{
    IntegrationVector d_vector;
    BarycentricCoord Oumuamua_coordinates = current_condition.get_barycentric();
    BarycentricCoord a; // acceleration

    int idx = int((current_condition.get_date().get_MJD() - this->date_start.get_MJD()) / STEP); // search for needed time

    for (std::string planet_name : this->planet_list)
    {
        BarycentricCoord planet_coordinates = planets->at(planet_name)[idx].get_barycentric();
        // Newton's formula:
        //                    -->
        //  a = dv / dt = GM * ri / |ri| ^ 3

        a = a + this->GM[planet_name] * (planet_coordinates - Oumuamua_coordinates) / help.POW_3((planet_coordinates - Oumuamua_coordinates).length());
    }

    d_vector.set_barycentric(current_condition.get_velocity().get_vx(), current_condition.get_velocity().get_vy(), current_condition.get_velocity().get_vz());
    d_vector.set_velocity(a.get_x(), a.get_y(), a.get_z());

    this->calculate_partial_derivates(&current_condition, d_vector.get_df_dx(), planets); // save partial derivates in df_dx
   
    /*
        df/db = (dv/dx0 dv/dv0) = (0 0)
                (da/dx0 da/dv0)   (0 0)
    */

    // b = x0
    // dx/db = df/dx * dx/db + df/db = df/dx * dx/db + 0 
    Matrix dx_db = (*d_vector.get_df_dx()) * (*current_condition.get_dx_db());
    d_vector.set_dx_db(dx_db);

    return d_vector;
}



void Integration::calculate_partial_derivates(IntegrationVector* current_condition, Matrix* partial_derivates, std::map<std::string, std::vector<IntegrationVector>>* planets)
{
        /*
            This method used for calculate dF/dX = ( dv/dx, dv/dv ) = ( 0,    E)
                                                   ( da/dx, da/dv )   (da/dx, 0)
        */

        (*partial_derivates)[0][3] = 1;
        (*partial_derivates)[1][4] = 1;
        (*partial_derivates)[2][5] = 1;

        double dax[3] = { 0, 0, 0 }; // dax / dx, dax / dy, dax / dz
        double day[3] = { 0, 0, 0 }; // day / dx, day / dy, day / dx
        double daz[3] = { 0, 0, 0 }; // daz / dx, daz / dy, daz / dz

        int idx = int((current_condition->get_date().get_MJD() - this->date_start.get_MJD()) / STEP); // search for needed time
        BarycentricCoord Oumuamua_coordinates = current_condition->get_barycentric();
        for (std::string planet_name : this->planet_list)
        {
            BarycentricCoord planet_coordinates = planets->at(planet_name)[idx].get_barycentric();
            double length = (planet_coordinates - Oumuamua_coordinates).length();
            dax[0] = dax[0] - this->GM[planet_name] / help.POW_N(length, 3) + 3 * this->GM[planet_name] * help.POW_N(planet_coordinates.get_x() - Oumuamua_coordinates.get_x(), 2) / help.POW_N(length, 5);
            dax[1] = dax[1] + 3 * this->GM[planet_name] * (planet_coordinates.get_x() - Oumuamua_coordinates.get_x()) * (planet_coordinates.get_y() - Oumuamua_coordinates.get_y()) / help.POW_N(length, 5);
            dax[2] = dax[2] + 3 * this->GM[planet_name] * (planet_coordinates.get_x() - Oumuamua_coordinates.get_x()) * (planet_coordinates.get_z() - Oumuamua_coordinates.get_z()) / help.POW_N(length, 5);

            day[0] = day[0] + 3 * this->GM[planet_name] * (planet_coordinates.get_y() - Oumuamua_coordinates.get_y()) * (planet_coordinates.get_x() - Oumuamua_coordinates.get_x()) / help.POW_N(length, 5);
            day[1] = day[1] - this->GM[planet_name] / help.POW_N(length, 3) + 3 * this->GM[planet_name] * help.POW_N(planet_coordinates.get_y() - Oumuamua_coordinates.get_y(), 2) / help.POW_N(length, 5);
            day[2] = day[2] + 3 * this->GM[planet_name] * (planet_coordinates.get_y() - Oumuamua_coordinates.get_y()) * (planet_coordinates.get_z() - Oumuamua_coordinates.get_z()) / help.POW_N(length, 5);

            daz[0] = daz[0] + 3 * this->GM[planet_name] * (planet_coordinates.get_z() - Oumuamua_coordinates.get_z()) * (planet_coordinates.get_x() - Oumuamua_coordinates.get_x()) / help.POW_N(length, 5);
            daz[1] = daz[1] + 3 * this->GM[planet_name] * (planet_coordinates.get_z() - Oumuamua_coordinates.get_z()) * (planet_coordinates.get_y() - Oumuamua_coordinates.get_y()) / help.POW_N(length, 5);
            daz[2] = daz[2] - this->GM[planet_name] / help.POW_N(length, 3) + 3 * this->GM[planet_name] * help.POW_N(planet_coordinates.get_z() - Oumuamua_coordinates.get_z(), 2) / help.POW_N(length, 5);
        }

        (*partial_derivates)[3][0] = dax[0];
        (*partial_derivates)[3][1] = dax[1];
        (*partial_derivates)[3][2] = dax[2];

        (*partial_derivates)[4][0] = day[0];
        (*partial_derivates)[4][1] = day[1];
        (*partial_derivates)[4][2] = day[2];

        (*partial_derivates)[5][0] = daz[0];
        (*partial_derivates)[5][1] = daz[1];
        (*partial_derivates)[5][2] = daz[2];
}



/*
    Method for numerical integrate
*/
std::vector<IntegrationVector> Integration::dormand_prince(IntegrationVector initial_condition, Date* start, Date* end, double step, std::map<std::string, std::vector<IntegrationVector>>* planets)
{
    IntegrationVector k1, k2, k3, k4, k5, k6, k7;
    std::vector<IntegrationVector> result;

    IntegrationVector new_condition = initial_condition;
    new_condition.set_date(*start);
    new_condition.get_dx_db()->make_identity(); // dx0/dx0 = E
    this->date_start = *start;

    for (double t = start->get_MJD(); t <= end->get_MJD() + step; t += step)
    {
        k1 = derivate_function(new_condition, planets);
        k2 = derivate_function(new_condition + step * (a21 * k1), planets);
        k3 = derivate_function(new_condition + step * (a31 * k1 + a32 * k2), planets);
        k4 = derivate_function(new_condition + step * (a41 * k1 + a42 * k2 + a43 * k3), planets);
        k5 = derivate_function(new_condition + step * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4), planets);
        k6 = derivate_function(new_condition + step * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5), planets);
        k7 = derivate_function(new_condition + step * (a71 * k1 + a72 * k2 + a73 * k3 + a74 * k4 + a75 * k5 + a76 * k6), planets);

        new_condition = new_condition + step * (b1 * k1 + b3 * k3 + b4 * k4 + b5 * k5 + b6 * k6);

        Date date;
        date.set_MJD(t);
        new_condition.set_date(date);
        result.push_back(new_condition);
    }

    return result;
}
