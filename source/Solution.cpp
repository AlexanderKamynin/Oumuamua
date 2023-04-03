#include "Solution.h"


Solution::Solution()
{
    // initial values was taken from here: https://ssd.jpl.nasa.gov/horizons/app.html#/
    initial_condition.set_barycentric_position(1.447364197925368E+08,  7.144824724390851E+07,  1.932516658038223E+07);
    initial_condition.set_velocity((4.674215462585911E+01) * 86400, (3.416179591144564E+00) * 86400, (1.683438620732583E+01) * 86400); // km/c -> km/day

    Converter converter;
    this->converter = converter;
    LightCorrector light_corrector = { &this->converter };
    this->light_corrector = light_corrector;
}


void Solution::read_data()
{
    data_reader.read_earth_rotation();
    data_reader.read_observations();
    data_reader.read_observatory_data();
    data_reader.read_hubble_data();
    data_reader.read_interpolation_time_data();


    data_reader.read_interpolation_center_planet("./input_data/earth.txt", "earth");
    data_reader.read_interpolation_center_planet("./input_data/sun.txt", "sun");
    data_reader.read_interpolation_center_planet("./input_data/jupyter.txt", "jupiter");
}


// convert time, celestial -> spherical and spherical -> geocentric coordinates
void Solution::convert_observations()
{
    std::vector<Observation>* data = data_reader.get_observations();
    for (int ind = 0; ind < data->size(); ind++) 
    {
        converter.UTC_to_TT(data->at(ind).get_date());
        converter.hours_to_degrees_system(data_reader.get_observation(ind));
        converter.spherical_to_geocentric(data_reader.get_observation(ind));
    }
     converter.interpolation_time(data_reader.get_observations()->at(0).get_date(), data, data_reader.get_interpolation_time());
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
    double step = 0.2; // MJD step
    std::vector<IntegrationVector> model_measures;
    std::vector<IntegrationVector> base_measures;
    std::vector<IntegrationVector> model_orbits;

    std::vector<SphericalCoord> model_spherical;
    std::vector<SphericalCoord> base_spherical;

    std::map<std::string, std::vector<IntegrationVector>> map_planets = converter.interpolation_center_planet(data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), step, data_reader.get_interpolation_planets());

    model_orbits = integration.dormand_prince(initial_condition, data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), step, &map_planets);
    model_measures = converter.interpolation_model_on_grid(data_reader.get_observations_vector(), data_reader.get_observations()->at(0).get_date(), model_orbits);

    converter.geo_to_bary_for_base_measure(data_reader.get_observations(), data_reader.get_obsevatory_map(), data_reader.get_earth_rotation_vector(), data_reader.get_interpolation_hubble(), map_planets["earth"]);
    // light time correction, gravitational deflection, abberation
    light_corrector.light_time_correction(data_reader.get_observations(), data_reader.get_obsevatory_map(), &model_measures, &map_planets["sun"]);

    for (int i = 0; i < (data_reader.get_observations_vector()).size(); i++) 
    {
        IntegrationVector base_vector;
        base_vector.set_date(*data_reader.get_observations_vector()[i].get_date());
        base_vector.set_barycentric_position(data_reader.get_observations_vector()[i].get_barycentric().get_alpha(), data_reader.get_observations_vector()[i].get_barycentric().get_beta(), data_reader.get_observations_vector()[i].get_barycentric().get_gamma());
        base_measures.push_back(base_vector);
    }

    for (int i = 0; i < model_measures.size(); i++)
    {
        converter.barycentric_to_spherical(&model_measures[i], &model_spherical);
        converter.barycentric_to_spherical(&base_measures[i], &base_spherical);
    }

    write_result(model_measures, base_measures, model_spherical, base_spherical);
}


void Solution::write_result(std::vector<IntegrationVector> model, std::vector<IntegrationVector> base_measures, std::vector<SphericalCoord> model_spherical, std::vector<SphericalCoord> base_spherical)
{
    std::ofstream model_out;
    model_out.open(model_file);

    int counter = 0;
    if (model_out.is_open())
    {
        for (int ind = 0; ind < model.size(); ind++)
        {
            counter += 1;
            model_out << std::setprecision(9) << model[ind].get_date().get_MJD() << "\tRA= " << model_spherical[ind].get_right_ascension() << "\tDEC= " << model_spherical[ind].get_declination() <<
                "\tvx(km/s)= " << model[ind].get_velocity().get_vx() / 86400 << "\tvy(km/s)= " << model[ind].get_velocity().get_vy() / 86400 << "\tvz(km/s)= " << model[ind].get_velocity().get_vz() / 86400 << '\n';
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
            base_out << std::setprecision(9) << base_measures[ind].get_date().get_MJD() << "\tRA= " << base_spherical[ind].get_right_ascension() << "\tDEC= " << base_spherical[ind].get_declination() << "\n";
        }
        base_out.close();
        std::cout << "Base:: " << counter << " strings was written in the file {" + base_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }
}


/*
    Call all main method
*/
void Solution::act()
{
    read_data();
    convert_observations();
    convert_observatory();
    integrate();
}
