#pragma once
#include "./measure/Observation.h"
#include "./measure/Observatory.h"
#include "./measure/InterpolationTimeFrame.h"
#include "./measure/IntegrationVector.h"

#include "./measure/InterpolationHubbleFrame.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


//Класс, читающий данные из файлов
class DataReader {
private:
    std::vector<Observation> observations;

    std::map<std::string, Observatory> observatory;

    std::vector<InterpolationTimeFrame> interpolation_time;
    std::map<std::string, std::vector<IntegrationVector>> InterpolationPlanets;

    std::vector<InterpolationHubbleFrame> interpolation_hubble;

public:
    void read_observations(std::string filename = "./data/observations.txt");
    void read_observatory_data(std::string filename = "./data/observatory.txt");
    void read_hubble_data(std::string filename = "./data/hubble_data.txt");
    void read_interpolation_time_data(std::string filename = "./data/interpolation_time_data.txt");
    void read_interpolation_center_planet(std::string filename, std::string name);
    std::vector<InterpolationTimeFrame> get_interpolation_time();

    std::vector<IntegrationVector> get_interpolation_earth();
    std::map<std::string, std::vector<IntegrationVector>> get_interpolation_planets();
    std::vector<IntegrationVector>* get_planet_by_name(std::string);

    std::vector<Observation>* get_observations();
    Observation* get_observation(int);

    std::vector<Observation> get_observations_vector();
    std::map<std::string, Observatory> get_observatory();

    std::vector<InterpolationHubbleFrame> get_interpolation_hubble();
    Observatory* get_observatory_data_by_code(std::string);
    std::map<std::string, Observatory>* get_obsevatory_link();

};