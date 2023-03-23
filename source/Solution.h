#include "./data_processing/DataReader.h"
#include "./data_processing/Converter.h"
#include "./data_processing/Integration.h"


//Класс, вызывающий методы других классов
class Solution
{
private:
    Converter cnv;
    DataReader dhand;

    IntegrationVector x0;
    Integration integration;
public:
    Solution();
    void read_data();
    void convert_observations();
    void convert_observatory();
    void convert_interpolation_data();
    void integrate();
    void calculate_MNK(std::vector<IntegrationVector>, std::vector<IntegrationVector>);
    void write_to_file(std::vector<IntegrationVector>, std::vector<IntegrationVector>);
};