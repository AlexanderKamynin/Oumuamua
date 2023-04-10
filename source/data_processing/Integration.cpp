#include "Integration.h"


IntegrationVector Integration::derivate_function(IntegrationVector current_condition, std::map<std::string, std::vector<IntegrationVector>>* planets)
{
    IntegrationVector d_vector;
    BarycentricCoord a; // acceleration

    int idx = int((current_condition.get_date().get_MJD() - this->date_start.get_MJD()) / 0.03); // search for needed time

    for (std::string planet_name : this->planet_list)
    {
        BarycentricCoord planet_coordinates = planets->at(planet_name)[idx].get_barycentric_position();
        BarycentricCoord Oumuamua_coordinates = current_condition.get_barycentric_position();
        // Newton's formula:
        //                    -->
        //  a = dv / dx = GM * ri / ri ^ 3
        
        a = a + this->GM[planet_name] * (planet_coordinates - Oumuamua_coordinates) / help.POW_3((planet_coordinates - Oumuamua_coordinates).length());
    }

    d_vector.set_barycentric_position(current_condition.get_velocity().get_vx(), current_condition.get_velocity().get_vy(), current_condition.get_velocity().get_vz());
    d_vector.set_velocity(a.get_alpha(), a.get_beta(), a.get_gamma());

    return d_vector;
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
    result.push_back(new_condition);
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
