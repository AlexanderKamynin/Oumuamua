#pragma once
#include "./data_processing/DataReader.h"
#include "./data_processing/Converter.h"
#include "./data_processing/Integration.h"
#include "./data_processing/LightCorrector.h"
#include "./data_processing/Interpolator.h"
#include "../source/measure/data_structures/ModelMeasure.h"
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
    Integration integration;
    Interpolator interpolator;
    LightCorrector light_corrector;


    IntegrationVector initial_condition;
    std::vector<ModelMeasure> model_measures;

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
    void direct_problem();
    void inverse_problem();
    void write_direct_problem_result(std::vector<IntegrationVector>* base_measures, std::vector<SphericalCoord>* base_spherical);
    std::vector<IntegrationVector> interolate_JPL();
};