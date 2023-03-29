#pragma once
#include "./data_processing/DataReader.h"
#include "./data_processing/Converter.h"
#include "./data_processing/Integration.h"
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
    // outputting data
    void write_result(std::vector<IntegrationVector>, std::vector<IntegrationVector>);
};