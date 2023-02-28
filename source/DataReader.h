#pragma once
#include "Observation.h"
#include "./measure/CylindricalCoord.h"

#include <iostream>
#include <fstream>
#include <vector>


//Класс, читающий данные из папки ./data/
class DataReader
{
private:
    std::vector<Observation> observations; //наблюдаемые величины
    std::vector<CylindricalCoord> observatory; //обсерватории
public:
    void read_observations(const std::string filename = "./data/observations.txt");
    void read_observatory_data(const std::string filename = "./data/observatory.txt");
};
