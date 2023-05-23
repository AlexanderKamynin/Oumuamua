#include "Solution.h"



Solution::Solution()
{
    // initial values was taken from here: https://ssd.jpl.nasa.gov/horizons/app.html#/

    initial_condition.set_barycentric(1.469591208242925E+08, 7.299762167917201E+07, 2.056299266163284E+07);
    initial_condition.set_velocity(3.859428549646102E+06, 3.244525935598258E+05, 1.492020244998816E+06); // km/day

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
    data_reader.read_interpolation_center_planet("./input_data/neptune.txt", "neptune");
    data_reader.read_interpolation_center_planet("./input_data/uranus.txt", "uranus");
}



// convert time, celestial -> spherical and spherical -> geocentric coordinates
void Solution::convert_observations()
{
    std::vector<Observation>* data = data_reader.get_observations();
    for (int i = 0; i < data->size(); i++) 
    {
        converter.UTC_to_TT(data->at(i).get_date(), data_reader.get_observation(i));
        converter.spherical_hours_to_spherical_radians(data_reader.get_observation(i));
        converter.barycentric_spherical_to_geocentric_cartesian(data_reader.get_observation(i));
    }

     for (int i = 0; i < data->size(); i++) 
     {
         ModelMeasure base_state;
         base_state.set_date(*data->at(i).get_date());
         base_state.set_spherical(data->at(i).get_spherical_position());
         this->base_measures.push_back(base_state);
     }
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
    Date start;
    start.set_MJD(START_DATE);
    model_orbits = integration.dormand_prince(initial_condition, &start, data_reader.get_observations()->at(221).get_date(), STEP, map_planets);
    light_corrector.light_correct(data_reader.get_observations(), &model_orbits, &this->model_measures, &map_planets->at("sun"), &map_planets->at("earth"), data_reader.get_earth_velocity_info());

    for (int i = 0; i < this->model_measures.size(); i++) {
        this->model_measures.at(i).set_date(*data_reader.get_observations()->at(i).get_date());

        Velocity velocity = this->interpolator.find_orbit_velocity(this->model_measures.at(i).get_date(), &model_orbits);
        this->model_measures.at(i).set_velocity(velocity);

        Matrix interpolated_dx_db = this->interpolator.interpolate_dx_db(this->model_measures.at(i).get_date(), &model_orbits);
        this->model_measures.at(i).set_dx_db(interpolated_dx_db);

        BarycentricCoord p_vector = this->model_measures.at(i).get_barycentric();
        GeocentricCoord geo_position;
        geo_position.set_x(p_vector.get_x() * EARTH_RADIUS);
        geo_position.set_y(p_vector.get_y() * EARTH_RADIUS);
        geo_position.set_z(p_vector.get_z() * EARTH_RADIUS);
        this->model_measures.at(i).set_geocentric(geo_position);
    }


    //for (int i = 0; i < this->model_measures.size(); i++)
    //{
    //    converter.barycentric_cartesian_to_geocentric_cartesian(&(this->model_measures[i]), &map_planets->at("earth"));
    //    converter.geocentric_cartesian_to_geocentric_spherical(&(this->model_measures[i]));
    //}


    write_direct_problem_result();
}



void Solution::write_direct_problem_result()
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

    if (base_out.is_open())
    {
        for (int ind = 0; ind < this->base_measures.size(); ind++)
        {
            counter += 1;
            base_out << std::setprecision(9) << this->base_measures.at(ind).get_date().get_MJD() << "\tRA= " << this->base_measures.at(ind).get_spherical().get_right_ascension() << "\tDEC= " << this->base_measures.at(ind).get_spherical().get_declination() << "\n";
        }
        base_out.close();
        //std::cout << "Base:: " << counter << " strings was written in the file {" + base_file + "}" << std::endl;
        counter = 0;
    }
    else
    {
        std::cout << "Error of writing file\n";
    }
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
    double W[444]; // W is diagonal matrix with 1/sigma^2 elements 
    Matrix A = Matrix(444, 6);

    double delta_RA = 0;
    double delta_DEC = 0;
    double delta_RA_sum = 0;
    double delta_DEC_sum = 0;


    for (int i = 0; i < this->model_measures.size(); i++)
    {
        this->mnk.calculate_dg_dx(&this->model_measures[i]);
        this->mnk.calculate_dr_db(&this->model_measures[i]);

        delta_RA = this->base_measures[i].get_spherical().get_right_ascension() - this->model_measures[i].get_spherical().get_right_ascension();
        delta_DEC = this->base_measures[i].get_spherical().get_declination() - this->model_measures[i].get_spherical().get_declination();

        // normilize delta RA to [-pi;pi]
        while ((delta_RA > PI) or (delta_RA < -PI))
        {
            int sign = delta_RA > PI ? -1 : 1;
            delta_RA = delta_RA + sign * 2 * PI;
        }

        delta_RA_sum += delta_RA * delta_RA;
        delta_DEC_sum += delta_DEC * delta_DEC;

        for (int j = 0; j < 6; j++)
        {
            A[2 * i][j] = (*this->model_measures[i].get_dr_db())[0][j];  // RA row in matrix
            A[2 * i + 1][j] = (*this->model_measures[i].get_dr_db())[1][j]; // the next is DEC row
        }

        R[2 * i][0] = delta_RA;
        R[2 * i + 1][0] = delta_DEC;

        double accuracy = 1e6;
        //���� ���������� ���� � ��������� N ������, �� ������ �������, ��� ����������� ���������� ����� ���������� �������� ������� � (N+1)-� �����.
        int last_digit = int(delta_RA * accuracy * 10) % 10;
        double w_ra = last_digit != 0 ? (2 * last_digit) / (accuracy * 10) : (2 * (last_digit + 1)) / (accuracy * 10);
        last_digit = int(delta_DEC * accuracy * 10) % 10;
        double w_dec = last_digit != 0 ? (2 * last_digit) / (accuracy * 10) : (2 * (last_digit + 1)) / (accuracy * 10);
        W[2 * i] = 1.0 / (w_ra * w_ra);
        W[2 * i + 1] = 1.0 / (w_dec * w_dec);
    }

    this->wrms.first = std::sqrt(delta_RA_sum / this->model_measures.size()); // RA
    this->wrms.second = std::sqrt(delta_DEC_sum / this->model_measures.size()); // DEC

    // Gauss-Newton
    this->initial_condition = this->mnk.Gauss_Newton(this->initial_condition, &A, &R, W);
}


/*
    Call all main method
*/
void Solution::act()
{
    std::cout << "\t>>----- Reading data -----<<\n\n";
    read_data();

    convert_observations();
    convert_observatory();
    std::map<std::string, std::vector<IntegrationVector>>* map_planets = data_reader.get_interpolation_planets();
    converter.set_tdb_grid(data_reader.get_interpolation_time());
    converter.cartesian_geocentric_to_cartesian_barycentric(data_reader.get_observations(), data_reader.get_obsevatory_map(), data_reader.get_earth_rotation_vector(), data_reader.get_interpolation_hubble(), map_planets->at("earth"));

    int iteration = 1;
    double accuracy = 1e-8;
    std::pair<double, double> old_wrms = { 0, 0 };


    std::cout << "\t>>----- Inverse problem -----<<\n\n";
    while (true)
    {
        old_wrms = this->wrms;
        direct_problem(map_planets);
        if (iteration == 1) {
            std::ofstream before_mnk;
            before_mnk.open("./output_data/before_mnk.txt");
            for (int i = 0; i < this->model_measures.size(); i++) {
                double RA_delta = std::fabs(this->model_measures[i].get_spherical().get_right_ascension() - this->base_measures[i].get_spherical().get_right_ascension());
                double DEC_delta = std::fabs(this->model_measures[i].get_spherical().get_declination() - this->base_measures[i].get_spherical().get_declination());
                before_mnk << RA_delta << " " << DEC_delta << "\n";
            }
        }

        inverse_problem();
        std::cout << "\t>>~~~~~Iteration [" << iteration << "]~~~~~<<\n";
        std::cout << "RA wrms delta: [" << std::fabs(this->wrms.first - old_wrms.first) << "]\n";
        std::cout << "DEC wrms delta: [" << std::fabs(this->wrms.second - old_wrms.second) << "]\n";

        if (std::abs(this->wrms.first - old_wrms.first) <= accuracy and std::abs(this->wrms.second - old_wrms.second) <= accuracy)
        {
            std::cout << "\n\t>>----- Inverse problem done -----<<\n\n";
            std::cout << "Complete weighted-root-mean-square values is equal for RA=[" << this->wrms.first << "], for DEC=[" << this->wrms.second << "]\n";
            std::cout << "Difference between real and model measures:\n";
            std::ofstream after_mnk;
            after_mnk.open("./output_data/after_mnk.txt");
            for (int i = 0; i < this->model_measures.size(); i++)
            {
                double RA_delta = std::fabs(this->model_measures[i].get_spherical().get_right_ascension() - this->base_measures[i].get_spherical().get_right_ascension());
                double DEC_delta = std::fabs(this->model_measures[i].get_spherical().get_declination() - this->base_measures[i].get_spherical().get_declination());
                std::cout << "For observation " << i + 1 << " difference in RA=[" << RA_delta << "], in DEC=[" << DEC_delta << "]\n";

                after_mnk << RA_delta << " " << DEC_delta << "\n";
            }
            break;
        }
        iteration++;
        this->clear_space();
    }
}


void Solution::clear_space()
{
    this->model_measures.clear();
}