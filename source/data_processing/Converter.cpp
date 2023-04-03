#include "Converter.h"



/*
    Method that convert cylindrical coordinates into cartesian coordinates
    https://ru.wikipedia.org/wiki/Цилиндрическая_система_координат
    https://www.minorplanetcenter.net/iau/lists/ObsCodesF.html
*/
CartesianCoord Converter::cylindrical_to_cartesian(CylindricalCoord coordinates)
{
    CartesianCoord cartesian_coordinates;

    /*
        x = rho * cos (longitude) * cos (delta)
        y = rho * sin (longitude) * cos (delta)
        z = rho * sin (delta)
    */
    cartesian_coordinates.set_x(std::cos(coordinates.get_longitude()) * coordinates.get_cos() * EARTH_RADIUS);
    cartesian_coordinates.set_y(std::sin(coordinates.get_longitude()) * coordinates.get_cos() * EARTH_RADIUS);
    cartesian_coordinates.set_z(coordinates.get_sin() * EARTH_RADIUS);

    return cartesian_coordinates;
}



/*
    This method converts the date from UTC to TT via TAI.
    UTC <-> TAI <-> TT
*/
void Converter::UTC_to_TT(Date* date)
{
    //deltat double   TAI minus UTC, seconds
    double deltat;

    // Input: year, month, day, fraction of day -> return value in deltat
    int status = iauDat(date->get_year(), date->get_month(), date->get_day(), date->get_day_fraction(), &deltat);

    if (status != 0)
    {
        std::cout << "Something wrong in convert UTC date to TT, status was " << status
            << ". About this mistake you can read the description for iauDat function" << std::endl;
    }
    else
    {
        /*
            TT = TAI + 32.184,
            UTC -> MJD
            TT = MJD + (delta + 32.184s) / 86400;
        */
        date->set_TT(date->get_MJD() + (deltat + 32.184) / 86400); // TT in days
    }
}



/*
    Interpolation of time on a uniform grid for observations
*/
void Converter::interpolation_time(Date* date_start, std::vector<Observation>* observations, std::vector<InterpolationTime> time)
{
    // gotta take <time == 8300 == all times * 100> <-> <observations - 222, >
    double interpolation_time_term;
    int last = 0;
    for (int i = 0; i < observations->size(); i++)
    {
        for (int j = last; j < time.size(); j++)
        {
            if (observations->at(i).get_date()->get_MJD() < time[j].get_date().get_MJD())
            {
                double delta_t_obs = observations->at(i).get_date()->get_MJD() - date_start->get_MJD();
                delta_t_obs = delta_t_obs * 100;
                //std::cout << "delta : "<<delta_t_obs << "\tj : " << j <<"\t diff [j-delta]: " << j- delta_t_obs<< "\n";
                /*double j_j = 0;
                std::cout << "j: " << j << " j_j: " << j_j << " i:" << i << " obs{i}:"<< observations->at(i).get_date()->get_MJD() << " time{j}" << time[j].get_date().get_MJD()<<  "\n";
                std::cout << "correct time: " << time[j].get_date().get_MJD() << " - " << observations->at(i).get_date()->get_MJD() <<"\n";
                */
                last = j - 1;
                double f_current = time[j].get_TT_TDB();
                double f_previous = time[j - 1].get_TT_TDB();
                double t_current = time[j].get_date().get_MJD();
                double t_previous = time[j - 1].get_date().get_MJD();
                double t_interpolate = observations->at(i).get_date()->get_MJD();

                interpolation_time_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);
                /*  interpolation_time_term = TT - TDB
                    we want to get to TDB, so multiply on -1 both sides:
                    - interpolation_time_term = TDB - TT
                    add to both sides TT, then
                    TDB = TT - interpolation_time_term

                    TT in days, interpolation_time_term in ms, so we should interpolation_time_term / 86400000
                */
                double TDB = observations->at(i).get_date()->get_TT() - (interpolation_time_term / 86400000);
                observations->at(i).get_date()->set_TDB(TDB);
                break;
            }
        }
    }
}



/*
    Find Hubble data for given date
*/
GeocentricCoord Converter::find_needed_hubble_data(Date date, std::vector<HubbleData> hubble_data)
{
    for (int i = 0; i < hubble_data.size() - 1; i++)
    {
        if ((hubble_data[i].get_date() < date) and (hubble_data[i + 1].get_date() > date))
        {
            return hubble_data[i].get_geocentric();
        }
    }
};



/*
    Method for converting from cartesian to geocentric coordinates for observatories
*/
GeocentricCoord Converter::cartesian_to_geocentric(CartesianCoord position, Date date, EarthRotation earth_rotation)
{
    double geocentric_to_cartesian[3][3];

    double uta;
    double utb;
    /*  Given:
            utc1,utc2  double   UTC as a 2-part quasi Julian Date (Notes 1-4)
            dut1       double   Delta UT1 = UT1-UTC in seconds (Note 5)
        Returned:
            ut11,ut12  double   UT1 as a 2-part Julian Date (Note 6)
    */
    iauUtcut1(earth_rotation.get_MJD() + 2400000.5, 0, earth_rotation.get_UT1_UTC(), &uta, &utb);

    // Input: tta=TT, ttb=0, uta=UT1_UTC, utb=0, xp=0, yp=0 -> return matrix for converting
    iauC2t06a(date.get_TT(), 2400000.5, uta, utb, earth_rotation.get_x(), earth_rotation.get_y(), geocentric_to_cartesian);

    // transpose for convert from cartesian to geocentric
    this->help.transpose_matrix(geocentric_to_cartesian);

    /*
                   (x)
        (matrix) * (y)
                   (z)
    */
    double x = geocentric_to_cartesian[0][0] * position.get_x() + geocentric_to_cartesian[0][1] * position.get_y() + geocentric_to_cartesian[0][2] * position.get_z();
    double y = geocentric_to_cartesian[1][0] * position.get_x() + geocentric_to_cartesian[1][1] * position.get_y() + geocentric_to_cartesian[1][2] * position.get_z();
    double z = geocentric_to_cartesian[2][0] * position.get_x() + geocentric_to_cartesian[2][1] * position.get_y() + geocentric_to_cartesian[2][2] * position.get_z();

    GeocentricCoord geocentric_position;
    geocentric_position.set_x(x);
    geocentric_position.set_y(y);
    geocentric_position.set_z(z);

    return geocentric_position;
};


/*
    Сonvert spherical coordinates to geocentric
*/
void Converter::spherical_to_geocentric(Observation* observation)
{
    double cartesian_coord[3];

    iauS2c(observation->get_spherical_position().get_right_ascension(), observation->get_spherical_position().get_declination(), cartesian_coord);

    observation->set_geocentric(cartesian_coord[0] * EARTH_RADIUS, cartesian_coord[1] * EARTH_RADIUS, cartesian_coord[2] * EARTH_RADIUS);
}



/*
    Convert spherical coordinates from "hours-minutes-seconds" system to degrees system (radians)
*/
void Converter::hours_to_degrees_system(Observation* observation)
{
    // Convert from hours-system to degrees:
    // https://planetcalc.ru/7663/
    double* RA_in_hours_system = observation->get_spherical_position().get_RA_in_hours_system();
    double degrees = 15 * RA_in_hours_system[0];
    double arcminutes = 0.25 * RA_in_hours_system[1];
    double arcseconds = 0.25 * RA_in_hours_system[2];
    char sign;
    if (RA_in_hours_system[0] < 0)
    {
        sign = '-';
    }
    else
    {
        sign = '+';
    }

    arcminutes += int(arcseconds) / 60; // if arcseconds >= 60 then add to arcminutes
    arcseconds = arcseconds - int(arcseconds) / 60;

    degrees += int(arcminutes) / 60; // if arcminutes >= 60 then add to degrees
    arcminutes = arcminutes - int(arcminutes) / 60;

    double ascension;
    iauAf2a('+', degrees, arcminutes, arcseconds, &ascension);


    double* DEC_in_hours_system = observation->get_spherical_position().get_DEC_in_hours_system();

    if (DEC_in_hours_system[0] < 0)
    {
        sign = '-';
    }
    else
    {
        sign = '+';
    }

    degrees = 15 * DEC_in_hours_system[0];
    arcminutes = 0.25 * DEC_in_hours_system[1];
    arcseconds = 0.25 * DEC_in_hours_system[2];

    arcminutes += int(arcseconds) / 60; // if arcseconds >= 60 then add to arcminutes
    arcseconds = arcseconds - int(arcseconds) / 60;

    degrees += int(arcminutes) / 60; // if arcminutes >= 60 then add to degrees
    arcminutes = arcminutes - int(arcminutes) / 60;

    double declination;
    iauAf2a(sign, degrees, arcminutes, arcseconds, &declination);

    observation->set_spherical(ascension, declination);
}



/*
    Convert from barycentric to spherical coodinates
    Method for IntegrationVector
*/
void Converter::barycentric_to_spherical(IntegrationVector* vector, std::vector<SphericalCoord>* coords)
{
    double barycentric_coord[3] = { vector->get_barycentric_position().get_alpha(), vector->get_barycentric_position().get_beta(), vector->get_barycentric_position().get_gamma() };
    double right_ascension;
    double declination;

    iauC2s(barycentric_coord, &right_ascension, &declination);
    SphericalCoord temp_coords;
    temp_coords.set_declination(declination);
    temp_coords.set_right_ascension(right_ascension);
    coords->push_back(temp_coords);
}


/*
    Convert geocentric to barycentric coordinates
    Used for base observation
*/
void Converter::geo_to_bary_for_base_measure(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<EarthRotation>* earth_rotation, std::vector<HubbleData> hubble_data, std::vector<IntegrationVector> earth_position)
{
    Date* start_date = observations->at(0).get_date();
    for (int i = 0; i < observations->size(); i++)
    {
        BarycentricCoord observatory_position;
        GeocentricCoord Oumuamua_position = observations->at(i).get_geocentric();
        Date* current_date = observations->at(i).get_date();

        if (observations->at(i).get_code() != "250") // 250 - Hubble code
        {
            Observatory current_observatory = observatory->at(observations->at(i).get_code());

            // find earth rotation info for current date observation
            EarthRotation earth_rotation_info;
            for (int j = 0; j < earth_rotation->size(); j++) {
                if (earth_rotation->at(j).get_MJD() >= current_date->get_MJD()) {
                    earth_rotation_info = earth_rotation->at(j);
                    break;
                }
            }

            // convert observatory coordinates from cartesian to geocentric
            GeocentricCoord geocentric_observatory_position = cartesian_to_geocentric(current_observatory.get_cartesian(), *current_date, earth_rotation_info);
            // interpolation observatory coordinates to Earth center
            //@CHANGES [barycentric position of the center of the Earth] + [celestial geocentric position of the observatory]
            BarycentricCoord interpolated_Earth_center = interpolation_Earth_center(*current_date, *start_date, earth_position);
            observatory_position.set_alpha(interpolated_Earth_center.get_alpha() + geocentric_observatory_position.get_x());
            observatory_position.set_beta(interpolated_Earth_center.get_beta() + geocentric_observatory_position.get_y());
            observatory_position.set_gamma(interpolated_Earth_center.get_gamma() + geocentric_observatory_position.get_z());
        }
        else
        {
            GeocentricCoord geocentric_hubble_position = find_needed_hubble_data(*current_date, hubble_data);
            //@CHANGES [barycentric position of the center of the Earth] + [celestial geocentric position of the observatory]
            BarycentricCoord interpolated_Earth_center = interpolation_Earth_center(*current_date, *start_date, earth_position);
            observatory_position.set_alpha(interpolated_Earth_center.get_alpha() + geocentric_hubble_position.get_x());
            observatory_position.set_beta(interpolated_Earth_center.get_beta() + geocentric_hubble_position.get_y());
            observatory_position.set_gamma(interpolated_Earth_center.get_gamma() + geocentric_hubble_position.get_z());
        }

        // calculate Oumuamua position
        double alpha = observatory_position.get_alpha() + Oumuamua_position.get_x();
        double beta = observatory_position.get_beta() + Oumuamua_position.get_y();
        double gamma = observatory_position.get_gamma() + Oumuamua_position.get_z();

        // set calculated Oumuamua position in the barycentric system for base measure
        observations->at(i).set_barycentric(alpha, beta, gamma);
    }
}



/*
    Interpolation numerical integraion result (model) on the time grid
*/
std::vector<IntegrationVector> Converter::interpolation_model_on_grid(std::vector<Observation> observation_vector, Date* date_start, std::vector<IntegrationVector> interpolation_orbits)
{
    int last = 0;
    std::vector<IntegrationVector> result;
    for (int i = 0; i < observation_vector.size(); i++)
    {
        IntegrationVector interpolated_vector;
        int j = int((observation_vector[i].get_date()->get_MJD() - date_start->get_MJD()) / 0.2) + 2;
        BarycentricCoord interpolated_position = interpolation_bary_helper(interpolation_orbits[j], interpolation_orbits[j - 1], *observation_vector[i].get_date());
        Date new_date = *observation_vector[i].get_date();
        interpolated_vector.set_date(new_date);
        interpolated_vector.set_barycentric_position(interpolated_position.get_alpha(), interpolated_position.get_beta(), interpolated_position.get_gamma());
        interpolated_vector.set_velocity(interpolation_orbits[j].get_velocity().get_vx(), interpolation_orbits[j].get_velocity().get_vy(), interpolation_orbits[j].get_velocity().get_vz());
        result.push_back(interpolated_vector);
    }
    return result;
}


/*
    Interpolation all center planets coordinates
*/
std::map<std::string, std::vector<IntegrationVector>> Converter::interpolation_center_planet(Date* date_start, Date* date_end, double step, std::map<std::string, std::vector<IntegrationVector>> planets_position)
{
    std::map<std::string, std::vector<IntegrationVector>> interpolated_planet;
    for (auto interpolation_planet : planets_position)
    {
        Date current_date = *date_start;
        int last = 0;
        std::vector<IntegrationVector> interpolated_center_planet;
        while (current_date.get_MJD() < date_end->get_MJD() + 1)
        {
            for (int j = last; j < interpolation_planet.second.size(); j++)
            {
                if (current_date.get_MJD() < interpolation_planet.second[j].get_date().get_MJD())
                {
                    //std::cout << "interpolation_planet.second[j].get_date()" << interpolation_planet.second[j].get_date().get_MJD() << "\n";
                    /*std::cout << "(" << current_date.get_MJD() << "-" << date_start->get_MJD() << ")/0.2 + 1 == " << (current_date.get_MJD() - date_start->get_MJD()) / 0.2 + 1 << std::endl;
                    std::cout << interpolation_planet.second[j].get_date().get_MJD() << " - " << current_date.get_MJD() << "\n";*/
                    last = j - 1;
                    BarycentricCoord interpolated_position_1 = interpolation_bary_helper(interpolation_planet.second[j], interpolation_planet.second[j - 1], current_date);
                    IntegrationVector interpolated_position;
                    interpolated_position.set_date(current_date);
                    interpolated_position.set_barycentric_position(interpolated_position_1.get_alpha(), interpolated_position_1.get_beta(), interpolated_position_1.get_gamma());
                    interpolated_center_planet.push_back(interpolated_position);
                    current_date.set_MJD(current_date.get_MJD() + step);
                    break;
                }
            }
        }
        interpolated_planet[interpolation_planet.first] = interpolated_center_planet;
    }

    return interpolated_planet;
}

/*
    Interpolation of Earth center coordinates
*/
BarycentricCoord Converter::interpolation_Earth_center(Date date_current, Date date_start, std::vector<IntegrationVector> earth_position)
{
    int i = int((date_current.get_MJD() - date_start.get_MJD()) / 0.2) + 1;
    BarycentricCoord interpolated_position = interpolation_bary_helper(earth_position[i], earth_position[i - 1], date_current);
    return interpolated_position;
}


BarycentricCoord Converter::interpolation_bary_helper(IntegrationVector position_previous, IntegrationVector position_current, Date date)
{
    double f_current = position_current.get_barycentric_position().get_alpha();
    double f_previous = position_previous.get_barycentric_position().get_alpha();
    double t_current = position_current.get_date().get_MJD();
    double t_previous = position_previous.get_date().get_MJD();
    double t_interpolate = date.get_MJD();

    double interpolation_alpha_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

    f_current = position_current.get_barycentric_position().get_beta();
    f_previous = position_previous.get_barycentric_position().get_beta();
    double interpolation_beta_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

    f_current = position_current.get_barycentric_position().get_gamma();
    f_previous = position_previous.get_barycentric_position().get_gamma();
    double interpolation_gamma_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

    BarycentricCoord interpolated_position;
    interpolated_position.set_alpha(interpolation_alpha_term);
    interpolated_position.set_beta(interpolation_beta_term);
    interpolated_position.set_gamma(interpolation_gamma_term);
    return interpolated_position;
}