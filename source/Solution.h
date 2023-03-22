#include "DataReader.h"
#include "Converter.h"
#include "Integration.h"

//Класс, вызывающий методы других классов
class Solution {
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
    void test_reading();
    void integrate();
    void least_squares(std::vector<IntegrationVector>, std::vector<IntegrationVector>);
    void write_to_file(std::vector<IntegrationVector>, std::vector<IntegrationVector>);
};