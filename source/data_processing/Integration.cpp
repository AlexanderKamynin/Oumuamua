#include "Integration.h"


IntegrationVector Integration::diff(double t, IntegrationVector asteroid, std::map<std::string, std::vector<IntegrationVector>> planets) 
{
    IntegrationVector d_vector;
    BarycentricCoord dx;
    BarycentricCoord dv;
    for (int i = 0; i < planets["earth"].size(); i++)
    {
        //@change get_date -> get_date
        if (planets["earth"][i].get_date().get_MJD() == t) 
        {
            //@change set, get julian_date -> set, get date
            d_vector.set_date(planets["earth"][i].get_date());
            //@change len -> length, get_position -> get_barycentric_position
            dx = sqrt((GM["sun"] * (planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position())) / ((planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position()).length() * (planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position())) + (GM["jupiter"] * (planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position()) / ((planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position()).length() * (planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position()))));
            //@change len -> length, get_position -> get_barycentric_position
            dv = (GM["sun"] * (planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position())) / (pow((planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position()).length(), 2) * (planets["sun"][i].get_barycentric_position() - asteroid.get_barycentric_position())) + (GM["jupiter"] * (planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position()) / (pow((planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position()).length(), 2) * (planets["jupiter"][i].get_barycentric_position() - asteroid.get_barycentric_position())));
            break;
        }
    }
    //@change get_x, y, z -> get_alpha, beta, gamma, set_position -> set_barycentric_position
    d_vector.set_barycentric_position(dx.get_alpha(), dx.get_beta(), dx.get_gamma());
    //@change get_x, y, z -> get_alpha, beta, gamma
    d_vector.set_velocity(dv.get_alpha(), dv.get_beta(), dv.get_gamma());
    return d_vector;
};


std::vector<IntegrationVector> Integration::dormand_prince(IntegrationVector y, Date* start, Date* end, double h, std::map<std::string, std::vector<IntegrationVector>> planets)
{
    IntegrationVector k1, k2, k3, k4, k5, k6, k7;
    std::vector<IntegrationVector> result;

    IntegrationVector new_y = y;
    //std::ofstream DP_output;
    //DP_output.open(this->output_filename);

    for (double t = start->get_MJD(); t <= end->get_MJD() + h; t += h) 
    {
        k1 = diff(t, new_y, planets);
        k2 = diff(t + c2 * h, new_y + h * (a21 * k1), planets);
        k3 = diff(t + c3 * h, new_y + h * (a31 * k1 + a32 * k2), planets);
        k4 = diff(t + c4 * h, new_y + h * (a41 * k1 + a42 * k2 + a43 * k3), planets);
        k5 = diff(t + c5 * h, new_y + h * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4), planets);
        k6 = diff(t + h, new_y + h * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5), planets);
        k7 = diff(t + h, new_y + h * (a71 * k1 + a72 * k2 + a73 * k3 + a74 * k4 + a75 * k5 + a76 * k6), planets);
        new_y = new_y + h * (b1 * k1 + b3 * k3 + b4 * k4 + b5 * k5 + b6 * k6);

        Date date;
        date.set_MJD(t);
        //@change set_date -> set_date
        new_y.set_date(date);

        //DP_output << "DP::" << "x==" << new_y.get_position().get_x() << ";\ty==" << new_y.get_position().get_y() << ";\t\tz==" << new_y.get_position().get_z() << std::endl;
        result.push_back(new_y);
    }
    //DP_output.close();
    //std::cout << "The result of the integration of the Oumuamua orbit is written in the file{" << this->output_filename << "}" << std::endl;
    return result;
};


BarycentricCoord Integration::sqrt(BarycentricCoord frame)
{
    //@TODO I want to move this method to BarycentricCoord class
    BarycentricCoord result;
    //@change set_x, get_x -> alpha
    result.set_alpha(std::sqrt(frame.get_alpha()));
    //@change set_y, get_y -> beta
    result.set_beta(std::sqrt(frame.get_beta()));
    //@change set_z, get_z -> gamma
    result.set_gamma(std::sqrt(frame.get_gamma()));
    return result;
}