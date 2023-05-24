#pragma once
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"
#include "../measure/time/InterpolationTime.h"
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
    std::vector<TT_TDB_obj> TT_TDB_data;
    std::map<std::string, std::vector<IntegrationVector>> planets_data;
    std::vector<IntegrationVector> hubble_data;
    std::vector<EarthRotation> earth_rotation;
    std::vector<IntegrationVector> earth_velocity;
    std::vector<IntegrationVector> JPL_measuare;

    Helpers help;

    std::string observations_file = "./input_data/observations.txt";
    std::string observatory_file = "./input_data/observatory.txt";
    std::string hubble_file = "./input_data/hubble_data.txt";
    std::string TT_TDB_file= "./input_data/interpolation_time_data.txt";
    std::string earth_rotation_file = "./input_data/earth_rotation_data.txt";
    std::string JPL_measuare_file = "./input_data/JPL_base_measuare.txt";

public:
    void read_observations();
    void read_JPL_base_measuare();
    void read_observatory_data();
    void read_hubble_data();
    void read_TT_TDB_data();
    void read_planet_data(std::string filename, std::string name);
    void read_earth_rotation();

    std::vector<TT_TDB_obj>* get_TT_TDB_data();

    std::vector<IntegrationVector> get_earth_data();
    std::map<std::string, std::vector<IntegrationVector>>* get_planets_data();
    std::vector<IntegrationVector>* get_planet_by_name(std::string);

    std::vector<Observation>* get_observations();
    Observation* get_observation(int);

    std::vector<Observation> get_observations_vector();
    std::vector<EarthRotation>* get_earth_rotation_vector();
    std::map<std::string, Observatory> get_observatory();

    std::vector<IntegrationVector>* get_hubble_data();
    Observatory* get_observatory_data_by_code(std::string);
    std::map<std::string, Observatory>* get_obsevatory_map();

    std::vector<IntegrationVector>* get_JPL();
    std::vector<IntegrationVector>* get_earth_velocity_info();
};