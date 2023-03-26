#include "Solution.h"
#include <iomanip>


Solution::Solution()
{
    initial_condition.set_barycentric_position(1.469662678584988E+08, 7.299822249002472E+07, 2.056575565443711E+07);
    initial_condition.set_velocity((4.466861553600886E+01) * 86400, (3.754895272084024E+00) * 86400, (1.726865669233104E+01) * 86400);
}


void Solution::read_data()
{
    data_reader.read_earth_rotation();
    data_reader.read_observations();
    data_reader.read_observatory_data();
    data_reader.read_hubble_data();

    data_reader.read_interpolation_time_data();

    data_reader.read_interpolation_center_planet("./input_data/interpolation_earth.txt", "earth");
    data_reader.read_interpolation_center_planet("./input_data/interpolation_sun.txt", "sun");
    data_reader.read_interpolation_center_planet("./input_data/interpolation_jupiter.txt", "jupiter");
}

// convert time, celestial -> spherical and spherical -> geocentric coordinates
void Solution::convert_observations()
{
    std::vector<Observation>* data = data_reader.get_observations();
    for (int ind = 0; ind < data->size(); ind++) 
    {
        converter.UTC_to_TT(data->at(ind).get_date());
        converter.celestial_to_spherical(data_reader.get_observation(ind));
        converter.spherical_to_geocentric(data_reader.get_observation(ind));
    }
    converter.interpolation_time(data, data_reader.get_interpolation_time());
}

// convert cylindrical -> cartesian position of observatory
void Solution::convert_observatory() 
{
    std::map<std::string, Observatory> data = data_reader.get_observatory();
    for (auto& item : data)
    {
        Observatory* cur_obs = data_reader.get_observatory_data_by_code(item.first);
        cur_obs->set_cartesian(converter.cylindrical_to_cartesian(cur_obs->get_cylindric()));
    }
}


void Solution::integrate() 
{
    std::vector<IntegrationVector> model_measures;
    std::vector<IntegrationVector> base_measures;
    std::vector<IntegrationVector> model_orbits;

    std::map<std::string, std::vector<IntegrationVector>> map_planets = converter.interpolation_center_planet(0.2, data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), data_reader.get_interpolation_planets());

    model_orbits = integration.dormand_prince(initial_condition, data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), 0.2, map_planets);
    model_measures = converter.interpolation_to_observation(data_reader.get_observations_vector(), model_orbits);


    converter.geocentric_to_barycentric(data_reader.get_observations(), data_reader.get_obsevatory_link(), data_reader.get_interpolation_hubble(), map_planets["earth"]);
    for (int i = 0; i < (data_reader.get_observations_vector()).size(); i++) 
    {
        IntegrationVector tmp;
        tmp.set_date(*data_reader.get_observations_vector()[i].get_date());
        tmp.set_barycentric_position(data_reader.get_observations_vector()[i].get_barycentric().get_alpha(), data_reader.get_observations_vector()[i].get_barycentric().get_beta(), data_reader.get_observations_vector()[i].get_barycentric().get_gamma());
        base_measures.push_back(tmp);
    }
    
    this->calculate_MNK(model_measures, base_measures);
}


void Solution::calculate_MNK(std::vector<IntegrationVector> model, std::vector<IntegrationVector> base_measures)
{

    for (int i = 0; i < model.size(); i++) 
    {
        converter.barycentric_to_spherical(&model[i]);
        converter.barycentric_to_spherical(&base_measures[i]);
    }

    write_to_file(model, base_measures);
}


void Solution::write_to_file(std::vector<IntegrationVector> model, std::vector<IntegrationVector> base_measures)
{
    std::ofstream model_out;
    model_out.open(model_file);
    int counter = 0;
    if (model_out.is_open()) 
    {
        for (int ind = 0; ind < model.size(); ind++)
        {
            counter += 1;
            model_out << std::setprecision(9) << model[ind].get_date().get_MJD() << " " << model[ind].get_spherical_position().get_longitude() << " " << model[ind].get_spherical_position().get_latitude() << "\n";
        }
        model_out.close();
        std::cout << "Model:: " << counter << " strings was written in the file {" + base_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }

    std::ofstream base_out;
    base_out.open(base_file);
    if (base_out.is_open()) 
    {
        for (int ind = 0; ind < base_measures.size(); ind++)
        {
            counter += 1;
            base_out << std::setprecision(9) << base_measures[ind].get_date().get_MJD() << " " << base_measures[ind].get_spherical_position().get_longitude() << " " << base_measures[ind].get_spherical_position().get_latitude() << "\n";
        }
        base_out.close();
        std::cout << "Base:: " << counter << " strings was written in the file {" + base_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }

    std::ofstream codes;
    codes.open("./output_data/code.txt");
    for (int ind = 0; ind < data_reader.get_observations()->size(); ind++) 
    {

        codes << data_reader.get_observation(ind)->get_code() << "\n";
    }
    codes.close();
}

void Solution::act()
{
    read_data();
    convert_observations();
    convert_observatory();
    integrate();
}
