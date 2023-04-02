#pragma once
#include "../measure/data_structures/IntegrationVector.h"
#include "DataReader.h"
#include "../Helpers.h"

#include <vector>
#include <map>
#include <fstream>
#include <math.h>
#include <string>


/*
    Class for numerical integration
*/
class Integration
{
private:
    /*
        Coefficients for the Butcher table of the Dormand-Prince method
    */
    double c2 = (1.0 / 5.0);
    double c3 = (3.0 / 10.0);
    double c4 = (4.0 / 5.0);
    double c5 = (8.0 / 9.0);
    double c6 = 1.0;
    double c7 = 1.0;

    double b1 = (35.0 / 384.0);
    double b2 = 0;
    double b3 = (500.0 / 1113.0);
    double b4 = (125.0 / 192.0);
    double b5 = (-2187.0 / 6784.0);
    double b6 = (11.0 / 84.0);
    double b7 = 0;

    double a21 = (1.0 / 5.0);
    double a31 = (3.0 / 40.0);
    double a32 = (9.0 / 40.0);
    double a41 = (44.0 / 45.0);
    double a42 = (-56.0 / 15.0);
    double a43 = (32.0 / 9.0);
    double a51 = (19372.0 / 6561.0);
    double a52 = (-25360.0 / 2187.0);
    double a53 = (64448.0 / 6561.0);
    double a54 = (-212.0 / 729.0);
    double a61 = (9017.0 / 3168.0);
    double a62 = (-355.0 / 33.0);
    double a63 = (-46732.0 / 5247.0);
    double a64 = (49.0 / 176.0);
    double a65 = (-5103.0 / 18656.0);
    double a71 = (35.0 / 384.0);
    double a72 = (0.0);
    double a73 = (500.0 / 1113.0);
    double a74 = (125.0 / 192.0);
    double a75 = (-2187.0 / 6784.0);
    double a76 = (11.0 / 84.0);

    // GM you can see here: https://iaaras.ru/en/dept/ephemeris/epm/2017/#5
    std::map<std::string, double> GM = { {"earth", 398600.43552 * help.POW_2(86400)}, {"sun", 132712440043.85333 * help.POW_2(86400)}, {"jupiter", 126712764.13345 * help.POW_2(86400)} }; // km^3 / sec^2 -> km^3 / day^2
    std::vector<std::string> planet_list = { "earth", "sun", "jupiter" };

    Helpers help;
    Date date_start;
public:
    IntegrationVector derivate_function(IntegrationVector, std::map<std::string, std::vector<IntegrationVector>>*);
    std::vector<IntegrationVector> dormand_prince(IntegrationVector, Date*, Date*, double, std::map<std::string, std::vector<IntegrationVector>>*);
};