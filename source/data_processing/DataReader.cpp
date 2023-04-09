#include "DataReader.h"


void DataReader::read_observations() 
{
    std::ifstream file(this->observations_file);
    std::string line;
    int counter = 0;

    if (file.is_open())
    {
        while (getline(file, line)) 
        {
            if (line[14] != 's') 
            {
                Date observation_date(line.substr(15, 17));
                Observation observation;
                observation.set_date(observation_date);
                observation.set_code(line.substr(77, 3));
                observation.set_ascension_from_string(line.substr(32, 12));
                observation.set_declination_from_string(line.substr(44, 12));

                //gotta delite
                if (counter < 1)
                {
                    counter += 1;
                    std::cout << "Observation: mjd-" << observation.get_date()->get_MJD() << " code-" << observation.get_code() << " RA-" << observation.get_spherical_position().get_right_ascension() << " D-" << observation.get_spherical_position().get_declination() << std::endl;
                    std::cout << "Observation: mjd-" << line.substr(15, 17) << " code-" << line.substr(77, 3) << " RA-" << line.substr(32, 12) << " D-" << line.substr(44, 12) << std::endl;

                }
                //

                observations.push_back(observation);
            }
           
        }
    }
    else
    {
        std::cout << "Error reading file! {" << observations_file << "}\n";
    }

    file.close();
    std::cout << "Observation read: " << observations.size() << " \n";
}



void DataReader::read_observatory_data()
{
    std::ifstream file(this->observatory_file);
    std::string line;
    int counter = 0;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            std::string code = line.substr(0, 3);
            observatory[code] = Observatory();

            CylindricalCoord observatory_position;
            observatory_position.set_longitude_from_string(line.substr(4, 9));
            observatory_position.set_cos_from_string(line.substr(13, 8));
            observatory_position.set_sin_from_string(line.substr(21, 9));

            observatory[code].set_cylindrical(observatory_position);
            
            //@gotta delite
            if (counter == 0)
            {
                counter += 1;
                std::cout << "Observatory: longi" << observatory_position.get_longitude() << " cos" << observatory_position.get_cos() << " sin" << observatory_position.get_sin() << std::endl;
                std::cout << "Observatory: longi" << line.substr(4, 9) << " cos" << line.substr(13, 8) << " sin" << line.substr(21, 9) << std::endl;
            }
            //
        }
    }
    else
    {
        std::cout << "Error reading file! {" << observatory_file << "}\n";
    }

    file.close();
    std::cout << "Observatory read: " << observatory.size() << " \n";
}



void DataReader::read_hubble_data()
{
    std::ifstream file(hubble_file);
    std::string line;
    int counter = 1;
    if (file.is_open())
    {
        while (getline(file, line))
        {
            GeocentricCoord hubble_position;
            hubble_position.set_from_string(line.substr(25, line.length() - 25));

            Date hubble_date(line.substr(0, 10));
            hubble_date.set_UTC_from_string(line.substr(11, 13));

            HubbleData hubble;
            hubble.set_date(hubble_date);
            hubble.set_geocentric(hubble_position);
            interpolation_hubble.push_back(hubble);

            //@gotta delite
            if (counter == 0)
            {
                counter += 1;
                std::cout << "Hybble: UTC" << hubble_date.get_MJD() << " HP" << hubble_position.get_x() << "_" << hubble_position.get_y() << "_" << hubble_position.get_z() << std::endl;
                std::cout << "Hybble: UTC" << line.substr(11, 13) << " HP" << line.substr(25, line.length() - 25) << std::endl;
            }
            //

        }
    }
    else
    {
        std::cout << "Error reading file! {" << hubble_file << "}\n";
    }

    file.close();
    std::cout << "Hubble read: " << interpolation_hubble.size() << " \n";
}



void DataReader::read_interpolation_time_data()
{
    std::ifstream file(interpolation_time_file);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line)) 
        {
            InterpolationTime time;
            Date observation_date(line.substr(0, 12));
            time.set_date(observation_date);
            time.set_TT_TDB(line.substr(13, 9));
            interpolation_time.push_back(time);
        }
    }
    else
    {
        std::cout << "Error reading file! {" << interpolation_time_file << "}\n";
    }

    file.close();
    std::cout << "Interpolation time read: " << interpolation_time.size() << " \n";
}



void DataReader::read_interpolation_center_planet(std::string filename, std::string name)
{
    std::ifstream file(filename);
    std::string line;
    int counter = 0;

    if (file.is_open())
    {
        std::vector<IntegrationVector> planet;
        while (getline(file, line))
        {
            IntegrationVector planet_data;
            Date date(line.substr(0, 13));
            planet_data.set_date(date);

            std::vector<double> planet_position = help.split(line.substr(13, line.length() - 13), ' ', '\0');
            planet_data.set_barycentric_position(planet_position[0], planet_position[1], planet_position[2]);
            planet.push_back(planet_data);
            
            //@gotta delite
            if (counter == 0)
            {
                counter += 1;
                std::cout << "Planet{"+name+"}: position0-" << planet_position[0] << " postion1-" << planet_position[1] << " position2" << planet_position[2] << std::endl;
            }
        }
        std::cout << "Planet <" << name << "> read " << planet.size() << " \n";
        InterpolationPlanets[name] = planet;
    }
    else
    {
        std::cout << "Error reading file! {" << filename << "}\n";
    }

    file.close();
}



void DataReader::read_earth_rotation()
{
    std::ifstream file(earth_rotation_file);
    std::string data_line;
    int counter = 0;

    if (file.is_open())
    {
        while (getline(file, data_line))
        {
            EarthRotation rotation;
            
            rotation.set_MJD(std::stoi(data_line.substr(14, 5)));
            rotation.set_x(std::stod(data_line.substr(22, 8)));
            rotation.set_y(std::stod(data_line.substr(33, 8)));
            rotation.set_UT1_UTC(std::stod(data_line.substr(44, 9)));
            //@gotta delite
            if (counter == 0)
            {
                counter += 1;
                std::cout << "Rotation: mjd-" << rotation.get_MJD() << " x-" << rotation.get_x() << " y-" << rotation.get_y() << " UT1_UTC-" << rotation.get_UT1_UTC() << std::endl;
            }
            //
            
            earth_rotation.push_back(rotation);
        }
    }
    else
    {
        std::cout << "Error reading file! {" << earth_rotation_file << "}\n";
    }

    file.close();
    std::cout << "Earth rotation information read: " << earth_rotation.size() << " \n";
}



std::vector<InterpolationTime> DataReader::get_interpolation_time() 
{
    return interpolation_time;
}


std::vector<IntegrationVector> DataReader::get_interpolation_earth() 
{
    return InterpolationPlanets["earth"];
}


std::vector<Observation>* DataReader::get_observations() 
{
    return &observations;
}


std::map<std::string, std::vector<IntegrationVector>> DataReader::get_interpolation_planets() 
{
    return InterpolationPlanets;
}


Observation* DataReader::get_observation(int idx) 
{
    return &observations[idx];
}


std::vector<Observation> DataReader::get_observations_vector()
{
    return observations;
}


std::vector<EarthRotation>* DataReader::get_earth_rotation_vector()
{
    return &this->earth_rotation;
}


std::map<std::string, Observatory> DataReader::get_observatory()
{
    return observatory;
}


std::vector<HubbleData> DataReader::get_interpolation_hubble() 
{
    return interpolation_hubble;
}


Observatory* DataReader::get_observatory_data_by_code(std::string code) 
{
    return &observatory[code];
}


std::vector<IntegrationVector>* DataReader::get_planet_by_name(std::string name) 
{
    if (InterpolationPlanets.find(name) != InterpolationPlanets.end()) 
    {
        return &InterpolationPlanets[name];
    }
    return nullptr;
}


std::map<std::string, Observatory>* DataReader::get_obsevatory_map() 
{
    return &observatory;
}
