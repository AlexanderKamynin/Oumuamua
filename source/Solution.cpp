#include "Solution.h"



Solution::Solution()
{
    // initial values was taken from here: https://ssd.jpl.nasa.gov/horizons/app.html#/
    initial_condition.set_barycentric(1.46966286538887E+08, 7.29982316871326E+07, 2.05657582369639E+07);
    initial_condition.set_velocity((4.467714995410097E+01) * 86400, (3.759100797623457E+00) * 86400, (1.726983438363074E+01) * 86400); // km/c -> km/day

    Interpolator interpolator;
    this->interpolator = interpolator;
    Converter converter = { &this->interpolator };
    this->converter = converter;
    LightCorrector light_corrector = { &this->converter, &this->interpolator };
    this->light_corrector = light_corrector;
}



void Solution::read_data()
{
    data_reader.read_earth_rotation();
    data_reader.read_earth_velocity();
    data_reader.read_observations();
    data_reader.read_observatory_data();
    data_reader.read_hubble_data();
    data_reader.read_interpolation_time_data();
    data_reader.read_JPL_base_mesuare();


    data_reader.read_interpolation_center_planet("./input_data/earth.txt", "earth");
    data_reader.read_interpolation_center_planet("./input_data/sun.txt", "sun");
    data_reader.read_interpolation_center_planet("./input_data/jupyter.txt", "jupiter");
    data_reader.read_interpolation_center_planet("./input_data/mars.txt", "mars");
    data_reader.read_interpolation_center_planet("./input_data/mercury.txt", "mercury");
    data_reader.read_interpolation_center_planet("./input_data/moon.txt", "moon");
    data_reader.read_interpolation_center_planet("./input_data/saturn.txt", "saturn");
    data_reader.read_interpolation_center_planet("./input_data/venus.txt", "venus");
}



// convert time, celestial -> spherical and spherical -> geocentric coordinates
void Solution::convert_observations()
{
    std::vector<Observation>* data = data_reader.get_observations();
    for (int ind = 0; ind < data->size(); ind++) 
    {
        converter.UTC_to_TT(data->at(ind).get_date());
        converter.spherical_hours_to_spherical_radians(data_reader.get_observation(ind));
        converter.barycentric_spherical_to_geocentric_cartesian(data_reader.get_observation(ind));
    }
     this->interpolator.interpolation_time(data_reader.get_observations()->at(0).get_date(), data, data_reader.get_interpolation_time());
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


void Solution::direct_problem(std::map<std::string, std::vector<IntegrationVector>>* map_planets)
{
    std::vector<IntegrationVector> model_orbits;
    std::vector<IntegrationVector> base_measures_vector = interolate_JPL();
    std::vector<SphericalCoord> base_spherical;

    model_orbits = integration.dormand_prince(initial_condition, data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), STEP, map_planets);
    light_corrector.light_correct(data_reader.get_observations(), &model_orbits, &map_planets->at("sun"), data_reader.get_earth_velocity_info());


    for (int i = 0; i < data_reader.get_observations()->size(); i++) {
        ModelMeasure new_state;
        new_state.set_barycentric(data_reader.get_observations()->at(i).get_barycentric());
        new_state.set_date(*data_reader.get_observations()->at(i).get_date());

        Velocity velocity = this->interpolator.find_orbit_velocity(new_state.get_date(), &model_orbits);
        new_state.set_velocity(velocity);

        Matrix interpolated_dx_db = this->interpolator.interpolate_dx_db(new_state.get_date(), &model_orbits);
        new_state.set_dx_db(interpolated_dx_db);
        this->model_measures.push_back(new_state);
    }


    for (int i = 0; i < this->model_measures.size(); i++)
    {
        converter.barycentric_cartesian_to_barycentric_spherical(&base_measures_vector[i], &base_spherical);
        converter.barycentric_cartesian_to_barycentric_spherical(&(this->model_measures.at(i)));
        ModelMeasure base_state;
        base_state.set_barycentric(base_measures_vector[i].get_barycentric());
        base_state.set_spherical(base_spherical[i]);
        this->base_measures.push_back(base_state);
    }


    write_direct_problem_result(&base_measures_vector, &base_spherical);
}



void Solution::write_direct_problem_result(std::vector<IntegrationVector>* base, std::vector<SphericalCoord>* base_spherical)
{
    std::ofstream model_out;
    std::ofstream model_barycentric_out;
    model_out.open(model_file);
    model_barycentric_out.open("./output_data/model_barycentric.txt");

    int counter = 0;
    if (model_out.is_open())
    {
        for (int ind = 0; ind < this->model_measures.size(); ind++)
        {
            counter += 1;
            model_out << std::setprecision(9) << this->model_measures[ind].get_date().get_MJD() << "\tRA= " << this->model_measures[ind].get_spherical().get_right_ascension() << "\tDEC= " << this->model_measures[ind].get_spherical().get_declination() << '\n';
            model_barycentric_out << std::setprecision(15) << this->model_measures[ind].get_date().get_MJD() << "\tx= " << this->model_measures[ind].get_barycentric().get_x() << "\ty= " << this->model_measures[ind].get_barycentric().get_y() <<
                "\tz= " << this->model_measures[ind].get_barycentric().get_z() << std::endl;
        }
        model_out.close();
        model_barycentric_out.close();
        //std::cout << "Model:: " << counter << " strings was written in the file {" + model_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }

   
    

    std::ofstream base_out;
    std::ofstream base_barycentric_out;
    base_out.open(base_file);
    base_barycentric_out.open("./output_data/base_barycentric.txt");

    if (base_out.is_open())
    {
        for (int ind = 0; ind < base->size(); ind++)
        {
            counter += 1;
            base_barycentric_out << std::setprecision(15) << base->at(ind).get_date().get_MJD() << "\tx= " << base->at(ind).get_barycentric().get_x() << "\ty= " << base->at(ind).get_barycentric().get_y() <<
               "\tz= " << base->at(ind).get_barycentric().get_z() << std::endl;
            base_out << std::setprecision(9) << base->at(ind).get_date().get_MJD() << "\tRA= " << base_spherical->at(ind).get_right_ascension() << "\tDEC= " << base_spherical->at(ind).get_declination() << "\n";
        }
        base_out.close();
        base_barycentric_out.close();
        //std::cout << "Base:: " << counter << " strings was written in the file {" + base_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }
}


std::vector<IntegrationVector> Solution::interolate_JPL()
{
    int last = 0;
    std::vector<IntegrationVector> result;
    //std::cout << "IN INTER JPL\n";

    for (int i = 0; i < data_reader.get_observations_vector().size(); i++)
    {
        for (int j = last; j < data_reader.get_JPL()->size(); j++)
        {
            if (data_reader.get_observations_vector().at(i).get_date()->get_MJD() < data_reader.get_JPL()->at(j).get_date()->get_MJD())
            {
                last = j;
                IntegrationVector current_vector;
                IntegrationVector previous_vector;
                current_vector.set_barycentric(data_reader.get_JPL()->at(j+1).get_barycentric().get_x(), data_reader.get_JPL()->at(j+1).get_barycentric().get_y(), data_reader.get_JPL()->at(j+1).get_barycentric().get_z());
                previous_vector.set_barycentric(data_reader.get_JPL()->at(j).get_barycentric().get_x(), data_reader.get_JPL()->at(j).get_barycentric().get_y(), data_reader.get_JPL()->at(j).get_barycentric().get_z());
                current_vector.set_date(*data_reader.get_JPL()->at(j + 1).get_date());
                previous_vector.set_date(*data_reader.get_JPL()->at(j).get_date());

                BarycentricCoord interpolated_position = interpolator.interpolation_helper(*data_reader.get_observations_vector()[i].get_date(), current_vector, previous_vector);

                IntegrationVector current;
                current.set_date(*data_reader.get_observations_vector()[i].get_date());

                current.set_barycentric(interpolated_position.get_x(), interpolated_position.get_y(), interpolated_position.get_z());
                result.push_back(current);
                break;
            }
            
        }
    }
    
    return result;
}



void Solution::inverse_problem()
{
    // method for solve inverse problem

    /* 
       step1: calculate dr / db
       step2: save dr/db in matrix A
       step3: calculate delta r
       step4: calculate matrix W
       step5: Gauss-Newton
    */

    Matrix R = Matrix(444, 1); // r(B) = real data - model data
    Matrix W = Matrix(444, 444); // W is diagoanal matrix with 1/
    Matrix A = Matrix(444, 6);
    double delta_RA = 0;
    double delta_DEC = 0;
    double delta_RA_sum = 0;
    double delta_DEC_sum = 0;

    for (int i = 0; i < this->model_measures.size(); i++)
    {
        this->mnk.calculate_dg_dx(&this->model_measures[i]);
        this->mnk.calculate_dr_db(&this->model_measures[i]);

        delta_RA = this->model_measures[i].get_spherical().get_right_ascension() - this->base_measures[i].get_spherical().get_right_ascension();
        delta_DEC = this->base_measures[i].get_spherical().get_declination() - this->model_measures[i].get_spherical().get_declination();
        delta_RA_sum += delta_RA * delta_RA;
        delta_DEC_sum += delta_DEC * delta_DEC;

        for (int j = 0; j < 6; j++)
        {
            A[2 * i][j] = - (*this->model_measures[i].get_dr_db())[0][j];  // RA row in matrix
            A[2 * i + 1][j] = (*this->model_measures[i].get_dr_db())[1][j]; // the next is DEC row
        }

        //@log
        /*if (i < 3)
        {
            std::cout << "Matrix dr/db was:\n" << *this->model_measures[i].get_dr_db() << '\n';
            std::cout << "Matrix A now:\n" << A << std::endl;
        }*/

        R[2 * i][0] = delta_RA;
        R[2 * i + 1][0] = delta_DEC;

        double accuracy = 1e15;
        //���� ���������� ���� � ��������� N ������, �� ������ �������, ��� ����������� ���������� ����� ���������� �������� ������� � (N+1)-� �����.
        double w_ra = 2 * this->model_measures[i].get_spherical().get_right_ascension() / accuracy / 10;
        double w_dec = 2 * this->model_measures[i].get_spherical().get_declination() / accuracy / 10;
        W[2 * i][2 * i] = 1.0 / (w_ra * w_ra);
        W[2 * i + 1][2 * i + 1] = 1.0 / (w_dec * w_dec);
    }

    this->wrms.first = std::sqrt(delta_RA_sum / this->model_measures.size()); // RA
    this->wrms.second = std::sqrt(delta_DEC_sum / this->model_measures.size()); // DEC

    //@log
    //std::cout << A << std::endl;
    //std::cout << R << std::endl;

    // Gauss-Newton
    this->initial_condition = this->mnk.Gauss_Newton(this->initial_condition, &A, &W, &R);
}


/*
    Call all main method
*/
void Solution::act()
{
    read_data();
    convert_observations();
    convert_observatory();
    std::map<std::string, std::vector<IntegrationVector>> map_planets = interpolator.interpolation_center_planet(data_reader.get_observations()->at(0).get_date(), data_reader.get_observations()->at(221).get_date(), STEP, data_reader.get_interpolation_planets());
    converter.cartesian_geocentric_to_cartesian_barycentric(data_reader.get_observations(), data_reader.get_obsevatory_map(), data_reader.get_earth_rotation_vector(), data_reader.get_interpolation_hubble(), map_planets["earth"]);


    int iteration = 1;
    double accuracy = 1e-15;
    std::pair<double, double> old_wrms = { 0, 0 };

    while (true)
    {
        old_wrms = this->wrms;
        direct_problem(&map_planets);
        inverse_problem();
        this->clear_space();
        iteration++;

        std::cout << "RA wrms delta: " << std::abs(this->wrms.first - old_wrms.first) << "\n";
        std::cout << "DEC wrms delta: " << std::abs(this->wrms.second - old_wrms.second) << "\n";
        if (std::abs(this->wrms.first - old_wrms.first) <= accuracy or std::abs(this->wrms.second - old_wrms.second) <= accuracy)
        {
            std::cout << std::setprecision(15) << "x0 with accuracy=" << accuracy << " now is equal\n";
            std::cout << "position:\n";
            this->initial_condition.get_barycentric().print();
            std::cout << "\nvelocity\n";
            this->initial_condition.get_velocity().print();
            std::cout << '\n';
            break;
        }
    }
}


void Solution::clear_space()
{
    this->model_measures.clear();
    this->base_measures.clear();
}