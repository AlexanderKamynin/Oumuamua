#pragma once
#include "./data_processing/DataReader.h"
#include "./data_processing/Converter.h"
#include "./data_processing/Integration.h"
#include "./data_processing/LightCorrector.h"
#include <iomanip>


/*
                Solution class is the mediator pattern class 
        Here partial solutions are combined into one general solution
*/
class Solution
{
private:
    Converter converter;
    DataReader data_reader;

    IntegrationVector initial_condition;
    Integration integration;
    LightCorrector light_corrector;
    
    std::string model_file = "./output_data/model_measure.txt";
    std::string base_file = "./output_data/base_measure.txt";
public:
    Solution();
    void act();
    // work with data; data inputers
    void read_data();
    void convert_observations();
    void convert_observatory();
    // integration
    void integrate();
    void write_result(std::vector<IntegrationVector> model, std::vector<IntegrationVector> base_measures, std::vector<SphericalCoord> model_spherical, std::vector<SphericalCoord> base_spherical);
};