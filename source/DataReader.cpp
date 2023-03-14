#include "DataReader.h"


//—читывание данных наблюдений
void DataReader::read_observations(std::string filename) {
    std::ifstream file(filename);
    std::string data_line;

    if (!file.is_open())
        std::cout << "‘айл с данными наблюдений не может быть открыт!\n";
    else
    {
        while (getline(file, data_line)) {
            if (data_line[14] != 's') {

                Observation data_frame;
                Date observation_date(data_line.substr(15, 17));
                observation_date.set_time_from_fraction();
                observation_date.set_JD();
                data_frame.set_julian_date(observation_date);
                data_frame.set_code(data_line.substr(77, 3));
                data_frame.set_ascension_from_string(data_line.substr(32, 12));
                data_frame.set_declination_from_string(data_line.substr(44, 12));

                observations.push_back(data_frame);
            }
        }
    }
    file.close();
    std::cout << "Observation succesfully read: " << observations.size() << " ";
}


//—читывание данных местоположений обсерваторий
void DataReader::read_observatory_data(std::string filename) {
    std::ifstream file(filename);
    std::string data_line;
    if (!file.is_open())
        std::cout << "‘айл с данными обсерваторий не может быть открыт!\n";
    else
    {
        while (getline(file, data_line)) {
            CylindricalCoord data_frame;
            data_frame.set_longitude_from_string(data_line.substr(5, 8));
            data_frame.set_cos_from_string(data_line.substr(13, 8));
            data_frame.set_sin_from_string(data_line.substr(21, 9));
            observatory.push_back(data_frame);
        }
    }
    file.close();
}


void DataReader::read_hubble_data(std::string filename) {
    std::ifstream file(filename);
    std::string data_line;
    if (!file.is_open())
        std::cout << "‘айл с данными ’аббла не может быть открыт!\n";
    else
    {
        while (getline(file, data_line)) {
            CartesianCoord data_frame;
            data_frame.set_from_string(data_line.substr(28, data_line.length()));
            Date hubble_date(data_line.substr(0, 11));
            hubble_date.set_time_from_string(data_line.substr(11, 25));
            hubble.set_data_frame(hubble_date, data_frame);
        }
    }
    file.close();

}


void DataReader::read_interpolation_time_data(std::string filename) {
    std::ifstream file(filename);
    std::string data_line;
    int ind = 0;
    if (!file.is_open())
        std::cout << "‘айл с данными дл€ интерпол€ции даты не может быть открыт!\n";
    else
    {
        while (getline(file, data_line)) {
            ind++;
            InterpolationTimeFrame data_frame;
            Date observation_date(data_line.substr(0, 12));
            observation_date.set_time_from_fraction();
            observation_date.set_JD();
            data_frame.set_julian_date(observation_date);
            data_frame.set_TT_TDB(data_line.substr(13, 9));
            interpolation_time.push_back(data_frame);
        }
    }
    file.close();
}


std::vector<InterpolationTimeFrame> DataReader::get_interpolation_time() {
    return interpolation_time;
}


std::vector<Observation> DataReader::get_observations() {
    return observations;
}