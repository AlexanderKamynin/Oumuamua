#include "DataReader.h"


//—читывание данных наблюдений
void DataReader::read_observations() {
    std::ifstream file(observations_file);
    std::string data_line;

    if (!file.is_open())
        std::cout << "Error reading file! {" << observations_file << "}\n";
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
    std::cout << "Observation Readed: " << observations.size() << " \n";
}


//—читывание данных местоположений обсерваторий
void DataReader::read_observatory_data()
{
    std::ifstream file(observatory_file);
    std::string data_line;
    if (!file.is_open())
        std::cout << "Error reading file! {" << observatory_file << "}\n";
    else
    {
        while (getline(file, data_line)) {
            std::string code = data_line.substr(0, 3);
            observatory[code] = Observatory();

            CylindricalCoord data_frame;
            data_frame.set_longitude_from_string(data_line.substr(4, 10));
            data_frame.set_cos_from_string(data_line.substr(13, 8));
            data_frame.set_sin_from_string(data_line.substr(21, 9));

            observatory[code].set_cylindrical(data_frame);
        }
    }
    file.close();
    std::cout << "Observatory Readed: " << observatory.size() << " \n";
}

//—читывание данных дл€ интерпол€ции положени€ ’аббла
void DataReader::read_hubble_data()
{
    std::ifstream file(hubble_file);
    std::string data_line;
    int counter = 0;
    if (!file.is_open())
        std::cout << "Error reading file! {" << hubble_file << "}\n";
    else
    {
        while (getline(file, data_line))
        {
            counter += 1;
            GeocentricCoord data_frame;
            data_frame.set_from_string(data_line.substr(25, data_line.length() - 25));
            Date hubble_date(data_line.substr(0, 10));
            hubble_date.set_time_from_string(data_line.substr(0, 18));
            InterpolationHubbleFrame frame;
            frame.set_date(hubble_date);
            frame.set_geocentric(data_frame);
            interpolation_hubble.push_back(frame);
        }
    }
    file.close();
    std::cout << "Hubble Readed: " << counter << " \n";
}

//—читывание данных дл€ интерпол€ции времени
void DataReader::read_interpolation_time_data()
{
    std::ifstream file(interpolation_time_file);
    std::string data_line;
    int counter = 0;
    if (!file.is_open())
        std::cout << "Error reading file! {" << interpolation_time_file << "}\n";
    else
    {
        while (getline(file, data_line)) {
            counter++;
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
    std::cout << "Interpolation Time Readed: " << counter << " \n";
}

#include <iomanip>

//—читывание данных дл€ интерпол€ции центра небесного тела
void DataReader::read_interpolation_center_planet(std::string filename, std::string name)
{
    std::ifstream file(filename);
    std::string data_line;
    double x;
    double y;
    double z;

    int ind = 0;

    if (!file.is_open())
        std::cout << "Error reading file! {" << filename << "}\n";
    else
    {
        std::vector<IntegrationVector> planet;
        while (getline(file, data_line)) {
            IntegrationVector data_frame;
            Date observation_date(data_line.substr(0, 13));
            observation_date.set_time_from_fraction();
            observation_date.set_JD();
            data_frame.set_julian_date(observation_date);

            int prev = 14;
            bool flag = false;
            int last = 0;
            bool all_three = false;

            for (int i = 0; i < 3; i++) {
                for (int j = prev; j < data_line.length() + 1; j++) {
                    if (data_line[j] != ' ' and data_line[j] != '\0') {
                        flag = true;
                    }
                    if (((data_line[j] == ' ' and data_line[j + 1] != ' ') or (data_line[j] == '\0')) and flag) {
                        last = j;
                        while (data_line[last - 1] == ' ') {
                            last--;
                        }
                        switch (i) {
                        case 0:
                            x = std::stod(data_line.substr(prev, last - prev));
                            break;
                        case 1:
                            y = std::stod(data_line.substr(prev, last - prev));
                            break;
                        case 2:
                            z = std::stod(data_line.substr(prev, last - prev));
                            all_three = true;
                            break;
                        default:
                            break;
                        }
                        if (all_three) {

                            data_frame.set_position(x, y, z);
                            planet.push_back(data_frame);
                            ind++;
                            all_three = false;
                        }
                        prev = j + 1;
                        flag = false;
                        break;
                    }

                }
            }
        }
        std::cout << "Plantet <" << name << "> Readed " << planet.size() << " \n";
        InterpolationPlanets[name] = planet;
    }
    file.close();
}


std::vector<InterpolationTimeFrame> DataReader::get_interpolation_time() {
    return interpolation_time;
}

std::vector<IntegrationVector> DataReader::get_interpolation_earth() {
    return InterpolationPlanets["earth"];
}

std::vector<Observation>* DataReader::get_observations() {
    return &observations;
}

std::map<std::string, std::vector<IntegrationVector>> DataReader::get_interpolation_planets() {
    return InterpolationPlanets;
}

Observation* DataReader::get_observation(int ind) {
    return &observations[ind];
}

std::vector<Observation> DataReader::get_observations_vector() {
    return observations;
}

std::map<std::string, Observatory> DataReader::get_observatory() {
    return observatory;
}

std::vector<InterpolationHubbleFrame> DataReader::get_interpolation_hubble() {
    return interpolation_hubble;
}

Observatory* DataReader::get_observatory_data_by_code(std::string code) {
    return &observatory[code];
}

std::vector<IntegrationVector>* DataReader::get_planet_by_name(std::string name) {
    if (InterpolationPlanets.find(name) != InterpolationPlanets.end()) {
        return &InterpolationPlanets[name];
    }
    return nullptr;
}

std::map<std::string, Observatory>* DataReader::get_obsevatory_link() {
    return &observatory;
}