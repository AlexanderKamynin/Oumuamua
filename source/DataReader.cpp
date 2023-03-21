#include "DataReader.h"


//—читывание данных наблюдений
void DataReader::read_observations(std::string filename)
{
    std::ifstream file(filename);
    std::string data_line;

    if (!file.is_open())
        std::cout << "‘айл с данными наблюдений не может быть открыт!\n";
    else
    {
        while (getline(file, data_line)) 
        {
            if (data_line[14] != 's')
            {

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
void DataReader::read_observatory_data(std::string filename) 
{
    std::ifstream file(filename);
    std::string data_line;
    if (!file.is_open())
        std::cout << "‘айл с данными обсерваторий не может быть открыт!\n";
    else
    {
        while (getline(file, data_line))
        {
            CylindricalCoord data_frame;
            data_frame.set_longitude_from_string(data_line.substr(5, 8));
            data_frame.set_cos_from_string(data_line.substr(13, 8));
            data_frame.set_sin_from_string(data_line.substr(21, 9));
            observatory.push_back(data_frame);
        }
    }
    file.close();
}

//void DataHandler::read_observatory_data(std::string filename) {
//    std::ifstream file(filename);
//    std::string data_line;
//    if (!file.is_open())
//        std::cout << "‘айл с данными обсерваторий не может быть открыт!\n";
//    else
//    {
//        while (getline(file, data_line)) {
//            std::string code = data_line.substr(0, 3);
//            observatory[code] = ObservatoryData();
//
//            CylindricalFrame data_frame;
//            data_frame.set_longitude_from_string(data_line.substr(4, 10));
//            data_frame.set_cos_from_string(data_line.substr(13, 8));
//            data_frame.set_sin_from_string(data_line.substr(21, 9));
//
//            observatory[code].set_cylindrical(data_frame);
//        }
//    }
//    file.close();
//}



void DataReader::read_hubble_data(std::string filename)
{
    std::ifstream file(filename);
    std::string data_line;
    if (!file.is_open())
        std::cout << "‘айл с данными ’аббла не может быть открыт!\n";
    else
    {
        while (getline(file, data_line))
        {
            CartesianCoord data_frame;
            data_frame.set_from_string(data_line.substr(28, data_line.length()));
            Date hubble_date(data_line.substr(0, 11));
            hubble_date.set_time_from_string(data_line.substr(11, 25));
            hubble.set_data_frame(hubble_date, data_frame);
        }
    }
    file.close();

}


void DataReader::read_interpolation_time_data(std::string filename) 
{
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


std::vector<InterpolationTimeFrame> DataReader::get_interpolation_time()
{
    return interpolation_time;
}


std::vector<Observation> DataReader::get_observations()
{
    return observations;
}


void DataReader::read_interpolation_center_planet(std::string filename, std::string name)
{
    std::ifstream file(filename);
    std::string data_line;
    double x = 0;
    double y = 0;
    double z = 0;
    
    int ind = 0;

    if (!file.is_open())
        std::cout << "‘айл с данными дл€ интерпол€ции центра небесного тела не может быть открыт!\n";
    else
    {
        std::vector<IntegrationVector> planet;
        while (getline(file, data_line)) 
        {
            IntegrationVector data_frame;
            Date observation_date(data_line.substr(0, 13));
            observation_date.set_time_from_fraction();
            observation_date.set_JD();
            data_frame.set_julian_date(observation_date);

            int prev = 14;
            bool flag = false;
            int last = 0;
            bool all_three = false;

            for (int i = 0; i < 3; i++) 
            {
                for (int j = prev; j < data_line.length() + 1; j++) 
                {
                    if (data_line[j] != ' ' and data_line[j] != '\0') 
                    {
                        flag = true;
                    }
                    if (((data_line[j] == ' ' and data_line[j + 1] != ' ') or (data_line[j] == '\0')) and flag)
                    {
                        last = j;
                        while (data_line[last - 1] == ' ')
                        {
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
                        if (all_three)
                        {
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
        std::cout << planet.size() << "\n";
        InterpolationPlanets[name] = planet;
    }
    file.close();
}


