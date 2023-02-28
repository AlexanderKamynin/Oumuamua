#pragma once
#include "Observation.h"
#include "./measure/CylindricalCoord.h"

#include <iostream>
#include <fstream>
#include <vector>


//�����, �������� ������ �� ����� ./data/
class DataReader
{
private:
    std::vector<Observation> observations; //����������� ��������
    std::vector<CylindricalCoord> observatory; //������������
public:
    void read_observations(const std::string filename = "./data/observations.txt");
    void read_observatory_data(const std::string filename = "./data/observatory.txt");
};
