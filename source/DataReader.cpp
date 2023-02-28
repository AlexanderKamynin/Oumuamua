#include "DataReader.h"


//Считывание данных наблюдений
void DataReader::read_observations(const std::string filename) {
    std::ifstream file(filename);
    std::string string_line;
    if (!file.is_open())
        std::cout << "Can't read the file! Filename was:" << filename << "\n";
    else
    {
        while (getline(file, string_line)) {
            if (string_line[14] != 's') { //считывание 222 данных
                Observation data;
                data.set_julian_date(Date(string_line.substr(15, 17))); //юлианская дата
                data.set_code(string_line.substr(77, 3)); //код обсерватории
                data.set_ascension_from_string(string_line.substr(32, 12)); //прямое восхождение
                data.set_declination_from_string(string_line.substr(44, 12)); //склонение

                observations.push_back(data);
            }
        }
    }
    file.close();
    std::cout << "Observation readed: " << observations.size() << " \n";
}


//Считывание данных местоположений обсерваторий
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
            data.set_longitude_from_string(string_line.substr(5, 8)); //долгота
            data.set_cos_from_string(string_line.substr(13, 8));
            data.set_sin_from_string(string_line.substr(21, 9));

            observatory.push_back(data);
        }
    }
    file.close();
    std::cout << "Observatory readed: " << observatory.size() << " \n";
}

