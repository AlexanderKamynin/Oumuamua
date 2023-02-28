#include "DataReader.h"


//���������� ������ ����������
void DataReader::read_observations(const std::string filename) {
    std::ifstream file(filename);
    std::string string_line;
    if (!file.is_open())
        std::cout << "Can't read the file! Filename was:" << filename << "\n";
    else
    {
        while (getline(file, string_line)) {
            if (string_line[14] != 's') { //���������� 222 ������
                Observation data;
                data.set_julian_date(Date(string_line.substr(15, 17))); //��������� ����
                data.set_code(string_line.substr(77, 3)); //��� ������������
                data.set_ascension_from_string(string_line.substr(32, 12)); //������ �����������
                data.set_declination_from_string(string_line.substr(44, 12)); //���������

                observations.push_back(data);
            }
        }
    }
    file.close();
    std::cout << "Observation readed: " << observations.size() << " \n";
}


//���������� ������ �������������� ������������
void DataReader::read_observatory_data(std::string filename) {
    std::ifstream file(filename);
    std::string string_line;
    if (!file.is_open())
        std::cout << "Can't read the file! Filename was:" << filename << "\n";
    else
    {
        while (getline(file, string_line))
        {
            CylindricalCoord data;
            data.set_longitude_from_string(string_line.substr(5, 8)); //�������
            data.set_cos_from_string(string_line.substr(13, 8));
            data.set_sin_from_string(string_line.substr(21, 9));

            observatory.push_back(data);
        }
    }
    file.close();
    std::cout << "Observatory readed: " << observatory.size() << " \n";
}

