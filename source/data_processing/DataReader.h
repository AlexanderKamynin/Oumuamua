#pragma once
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"

#include "../measure/time/InterpolationTime.h"

#include "../measure/data_structures/HubbleData.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/data_structures/EarthRotation.h"

#include "../Helpers.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>


//Класс, читающий данные из файлов
class DataReader 
{
private:
    std::vector<Observation> observations;
    std::map<std::string, Observatory> observatory;
    std::vector<InterpolationTime> interpolation_time;
    std::map<std::string, std::vector<IntegrationVector>> InterpolationPlanets;
    std::vector<HubbleData> interpolation_hubble;
    std::vector<EarthRotation> earth_rotation;

    Helpers help;

    std::string observations_file = "./input_data/observations.txt";
    std::string observatory_file = "./input_data/observatory.txt";
    std::string hubble_file = "./input_data/hubble_data.txt";
    std::string interpolation_time_file= "./input_data/interpolation_time_data.txt";
    std::string earth_rotation_file = "./input_data/earth_rotation_data.txt";

public:
    void read_observations();
    void read_observatory_data();
    void read_hubble_data();
    void read_interpolation_time_data();
    void read_interpolation_center_planet(std::string filename, std::string name);

    void read_earth_rotation();

    std::vector<InterpolationTime> get_interpolation_time();

    std::vector<IntegrationVector> get_interpolation_earth();
    std::map<std::string, std::vector<IntegrationVector>> get_interpolation_planets();
    std::vector<IntegrationVector>* get_planet_by_name(std::string);

    std::vector<Observation>* get_observations();
    Observation* get_observation(int);

    std::vector<Observation> get_observations_vector();
    std::vector<EarthRotation>* get_earth_rotation_vector();
    std::map<std::string, Observatory> get_observatory();

    std::vector<HubbleData> get_interpolation_hubble();
    Observatory* get_observatory_data_by_code(std::string);
    std::map<std::string, Observatory>* get_obsevatory_map();
};