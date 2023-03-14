#pragma once
#include "Observation.h"
#include "./measure/CylindricalCoord.h"
#include "./measure/CartesianCoord.h"
#include "./measure/InterpolationTimeFrame.h"
#include "HubbleInterpolator.h"

#include <iostream>
#include <fstream>
#include <vector>


//Класс, читающий данные из папки ./data/
class DataReader
{
private:
    std::vector<Observation> observations; //наблюдаемые величины
    std::vector<CylindricalCoord> observatory; //обсерватории
    std::vector<InterpolationTimeFrame> interpolation_time;
    HubbleInterpolator hubble;
public:
    void read_observations(const std::string filename = "./data/observations.txt");
    void read_observatory_data(const std::string filename = "./data/observatory.txt");
    void read_hubble_data(std::string filename = "./data/hubble_data.txt");
    void read_interpolation_time_data(std::string filename = "./data/interpolation_time_data.txt");
    std::vector<InterpolationTimeFrame> get_interpolation_time();
    std::vector<Observation> get_observations();
};
