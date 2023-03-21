#include "Integration.h"


std::vector<IntegrationVector> Integration::dormand_prince(IntegrationVector y, Date start, Date end, double h) 
{
    IntegrationVector k1, k2, k3, k4, k5, k6, k7;
    std::vector<IntegrationVector> result;
    for (int t = start.get_MJD(); t <= end.get_MJD(); t += h) 
    {
        k1 = diff(t, y);
        k2 = diff(t + c2 * h, y + h * (a21 * k1));
        k3 = diff(t + c3 * h, y + h * (a31 * k1 + a32 * k2));
        k4 = diff(t + c4 * h, y + h * (a41 * k1 + a42 * k2 + a43 * k3));
        k5 = diff(t + c5 * h, y + h * (a51 * k1 + a52 * k2 + a53 * k3 + a54 * k4));
        k6 = diff(t + h, y + h * (a61 * k1 + a62 * k2 + a63 * k3 + a64 * k4 + a65 * k5));
        k7 = diff(t + h, y + h * (a71 * k1 + a72 * k2 + a73 * k3 + a74 * k4 + a75 * k5 + a76 * k6));
        Date date;
        date.set_MJD(t);
        k7.set_julian_date(date);
        result.push_back(k7);
    }
    return result;
};


IntegrationVector Integration::diff(double t, IntegrationVector) 
{
    IntegrationVector penis = IntegrationVector();
    return penis;
};
