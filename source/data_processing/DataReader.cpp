#include "DataReader.h"


void DataReader::read_observations() 
{
    std::ifstream file(this->observations_file);
    int observation_number = 0;

    if (file.is_open())
    {
        while (!file.eof()) 
        {
            observation_number++;
            Observation observation;
            int year, month;
            double day, tmp;
            double RA_in_hours_systems[3];
            double DEC_in_degrees_systems[3];
            std::string observatory_code;
            file >> year >> month >> day;
            file >> RA_in_hours_systems[0] >> RA_in_hours_systems[1] >> RA_in_hours_systems[2];
            file >> DEC_in_degrees_systems[0] >> DEC_in_degrees_systems[1] >> DEC_in_degrees_systems[2];

            if (!((observation_number >= 183 && observation_number <= 192) || (observation_number >= 198 && observation_number <= 202) || (observation_number >= 208))) 
            {
                file >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp;
            }

            file >> observatory_code;

            std::string date_line;
            date_line = std::to_string(year) + " " + std::to_string(month) + " " + std::to_string(day);
            Date date(date_line);
            
            observation.set_date(date_line);
            observation.set_RA_in_hours_systems(RA_in_hours_systems[0], RA_in_hours_systems[1], RA_in_hours_systems[2]);
            observation.set_DEC_in_degrees_systems(DEC_in_degrees_systems[0], DEC_in_degrees_systems[1], DEC_in_degrees_systems[2]);
            observation.set_code(observatory_code);

            observations.push_back(observation);
        }
    }
    else
    {
        std::cout << "Error reading file! {" << observations_file << "}\n";
    }

    file.close();
    std::cout << "Observation read: " << observations.size() << " \n";
}


void DataReader::read_JPL_base_measuare()
{
    std::ifstream file(this->JPL_mesuare_file);
    std::string line;

    if (file.is_open())
    {
        while (getline(file, line))
        {
            Date JPL_date;
            JPL_date.set_MJD(std::stod(line.substr(0,17)) - 2400000.5);
            Observation observation;
            observation.set_date(JPL_date);
            double x = std::stod(line.substr(52, 21));
            double y = std::stod(line.substr(76, 21));
            double z = std::stod(line.substr(100, 21));
            observation.set_barycentric(x, y, z);
            JPL_mesuare.push_back(observation);
        }
    }
    else
    {
        std::cout << "Error reading file! {" << JPL_mesuare_file << "}\n";
    }
    file.close();
    std::cout << "JPL_mesuare read: " << JPL_mesuare.size() << " \n";
}



void DataReader::read_observatory_data()
{
    std::ifstream file(this->observatory_file);
    std::string line;

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
            //@gotta insert convertation from cylindrical to baricetrical coords!
            observatory[code].set_cylindrical(observatory_position);
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
        while (!file.eof()) 
        {
            InterpolationTime interpolation_time;
            Date date;
            double time, tdb_tt;
            file >> time >> tdb_tt;
            date.set_MJD(time - JD_TO_MJD);
            interpolation_time.set_date(date);
            interpolation_time.set_TT_TDB(tdb_tt);
            this->interpolation_time.push_back(interpolation_time);
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

    if (file.is_open())
    {
        std::vector<IntegrationVector> planet;
        while (!file.eof())
        {
            IntegrationVector planet_data;
            double time, x, y, z, vx, vy, vz;
            file >> time >> x >> y >> z >> vx >> vy >> vz;
            
            Date date;
            date.set_MJD(time - JD_TO_MJD);
            planet_data.set_date(date);
            planet_data.set_barycentric(x, y, z);
            planet.push_back(planet_data);

            if (name == "earth")
            {
                IntegrationVector earth_velocity_info;
                earth_velocity_info.set_date(date);
                earth_velocity_info.set_velocity(vx / 86400, vy / 86400, vz / 86400);
                this->earth_velocity.push_back(earth_velocity_info);
            }
        }
        std::cout << "Planet <" << name << "> read " << planet.size() << " \n";
        InterpolationPlanets[name] = planet;
    }
    else
    {
        std::cout << "Error reading file! {" << filename << "}\n";
    }
}


void DataReader::read_earth_rotation()
{
    std::ifstream file(this->observations_file);
    int observation_number = 0;

    if (file.is_open())
    {
        while (!file.eof())
        {
            observation_number++;
            if (!((observation_number >= 183 && observation_number <= 192) || (observation_number >= 198 && observation_number <= 202) || (observation_number >= 208)))
            {
                int year, month;
                double day, tmp, xp, yp, UT1_UTC;
                std::string code;
                file >> year >> month >> day;
                file >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp; // skip reading ra and dec and observatory data
                file >> xp >> yp >> UT1_UTC;
                file >> code;

                EarthRotation rotation;
                std::string date_line;
                date_line = std::to_string(year) + " " + std::to_string(month) + " " + std::to_string(day);
                Date date(date_line);

                rotation.set_MJD(date.get_MJD());
                rotation.set_x(xp);
                rotation.set_y(yp);
                rotation.set_UT1_UTC(UT1_UTC);

                earth_rotation.push_back(rotation);
            }
            else {
                int year, month;
                double day, tmp;
                std::string code;
                file >> year >> month >> day;
                file >> tmp >> tmp >> tmp >> tmp >> tmp >> tmp >> code;
            }
        }
    }
    else
    {
        std::cout << "Error reading file! {" << earth_rotation_file << "}\n";
    }

    file.close();
    std::cout << "Earth rotation information read: " << earth_rotation.size() << " \n";
}



std::vector<InterpolationTime>* DataReader::get_interpolation_time() 
{
    return &interpolation_time;
}


std::vector<IntegrationVector> DataReader::get_interpolation_earth() 
{
    return InterpolationPlanets["earth"];
}


std::vector<Observation>* DataReader::get_observations() 
{
    return &observations;
}


std::map<std::string, std::vector<IntegrationVector>>* DataReader::get_interpolation_planets() 
{
    return &this->InterpolationPlanets;
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

std::vector<Observation>* DataReader::get_JPL()
{
    return &this->JPL_mesuare;
}

std::vector<IntegrationVector>* DataReader::get_earth_velocity_info()
{
    return &this->earth_velocity;
}
