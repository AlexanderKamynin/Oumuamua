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
            we have TAI - UTC, so:
            TT = deltat + MJD + 32.184;

            for ease of calculation we use MJD instead of UTC

        */
        date->set_TT(date->get_MJD() + deltat + 32.184);
    }
}



/*
    Interpolation of time on a uniform grid for observations
*/
void Converter::interpolation_time(std::vector<Observation>* observations, std::vector<InterpolationTime> time) 
{
    double interpolation_time_term;
    int last = 0;

    for (int i = 0; i < observations->size(); i++) 
    {
        for (int j = last; j < time.size(); j++)
        {
            if (observations->at(i).get_date()->get_MJD() < time[j].get_date().get_MJD())
            {
                last = j - 1;
                double f_current = time[j].get_TT_TDB();
                double f_previous = time[j - 1].get_TT_TDB();
                double t_current = time[j].get_date().get_MJD();
                double t_previous = time[j - 1].get_date().get_MJD();
                double t_interpolate = observations->at(i).get_date()->get_MJD();

                interpolation_time_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);
                observations->at(i).get_date()->set_TDB_from_TT_TDB(interpolation_time_term);
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
    
    // Input: We use JD method, so tta=TT, ttb=0, uta=UT1_UTC, utb=0, xp=0, yp=0 -> return matrix for converting
    iauC2t06a(date.get_TT(), 0, earth_rotation.get_UT1_UTC(), 0, earth_rotation.get_x(), earth_rotation.get_y(), geocentric_to_cartesian);

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
    Interpolation of Earth center coordinates
*/
BarycentricCoord Converter::interpolation_Earth_center(Date date, GeocentricCoord obsevatory_position, std::vector<IntegrationVector> earth_position)
{
    double interpolation_alpha_term;
    double interpolation_beta_term;
    double interpolation_gamma_term;

    for (int i = 0; i < earth_position.size(); i++)
    {
        if (date.get_MJD() < earth_position[i].get_date().get_MJD())
        {
            double f_current = earth_position[i].get_barycentric_position().get_alpha();
            double f_previous = earth_position[i - 1].get_barycentric_position().get_alpha();
            double t_current = earth_position[i].get_date().get_MJD();
            double t_previous = earth_position[i - 1].get_date().get_MJD();
            double t_interpolate = date.get_MJD();
            interpolation_alpha_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

            f_current = earth_position[i].get_barycentric_position().get_beta();
            f_previous = earth_position[i - 1].get_barycentric_position().get_beta();
            interpolation_beta_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

            f_current = earth_position[i].get_barycentric_position().get_gamma();
            f_previous = earth_position[i - 1].get_barycentric_position().get_gamma();
            interpolation_gamma_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

            BarycentricCoord interpolated_position;
            interpolated_position.set_alpha(interpolation_alpha_term);
            interpolated_position.set_beta(interpolation_beta_term);
            interpolated_position.set_gamma(interpolation_gamma_term);
            return interpolated_position;
        }
    }
}



/*
    Interpolation all center planets coordinates
*/
std::map<std::string, std::vector<IntegrationVector>> Converter::interpolation_center_planet(Date* date_start, Date* date_end, double step, std::map<std::string, std::vector<IntegrationVector>> planets_position)
{
    double interpolation_alpha_term;
    double interpolation_beta_term;
    double interpolation_gamma_term;
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
                if (current_date.get_MJD() < (interpolation_planet.second[j].get_date()).get_MJD())
                {
                    last = j - 1;
                    double f_current = interpolation_planet.second[j].get_barycentric_position().get_alpha();
                    double f_previous = interpolation_planet.second[last].get_barycentric_position().get_alpha();
                    double t_current = interpolation_planet.second[j].get_date().get_MJD();
                    double t_previous = interpolation_planet.second[last].get_date().get_MJD();
                    double t_interpolate = current_date.get_MJD();

                    IntegrationVector interpolated_position;

                    interpolation_alpha_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                    f_current = interpolation_planet.second[j].get_barycentric_position().get_beta();
                    f_previous = interpolation_planet.second[last].get_barycentric_position().get_beta();
                    interpolation_beta_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                    f_current = interpolation_planet.second[j].get_barycentric_position().get_gamma();
                    f_previous = interpolation_planet.second[last].get_barycentric_position().get_gamma();
                    interpolation_gamma_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                    interpolated_position.set_date(current_date);
                    interpolated_position.set_barycentric_position(interpolation_alpha_term, interpolation_beta_term, interpolation_gamma_term);
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
void Converter::barycentric_to_spherical(IntegrationVector* vector) 
{
    double barycentric_coord[3] = { vector->get_barycentric_position().get_alpha(), vector->get_barycentric_position().get_beta(), vector->get_barycentric_position().get_gamma() };
    double right_ascension;
    double declination;

    iauC2s(barycentric_coord, &right_ascension, &declination);
    vector->set_spherical_position(right_ascension, declination);
}



/*
    Convert from barycentric to spherical coodinates
    Method for Observation
*/
void Converter::barycentric_to_spherical(Observation* vector)
{
    double barycentric_coord[3] = { vector->get_barycentric().get_alpha(), vector->get_barycentric().get_beta(), vector->get_barycentric().get_gamma() };
    double right_ascension;
    double declination;

    iauC2s(barycentric_coord, &right_ascension, &declination);
    vector->set_spherical(right_ascension, declination);
}



/*
    Convert geocentric to barycentric coordinates
    Used for base observation
*/
void Converter::geo_to_bary_for_base_measure(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<EarthRotation>* earth_rotation, std::vector<HubbleData> hubble_data, std::vector<IntegrationVector> earth_position) 
{
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
            observatory_position = interpolation_Earth_center(*current_date, geocentric_observatory_position, earth_position);
        }
        else
        {
            GeocentricCoord geocentric_hubble_position = find_needed_hubble_data(*current_date, hubble_data);
            observatory_position = interpolation_Earth_center(*current_date, geocentric_hubble_position, earth_position);
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
std::vector<IntegrationVector> Converter::interpolation_model_on_grid(std::vector<Observation> observation_vector, std::vector<IntegrationVector> interpolation_orbits) 
{
    double interpolation_alpha_term;
    double interpolation_beta_term;
    double interpolation_gamma_term;
    int last = 0;
    std::vector<IntegrationVector> result;

    for (int i = 0; i < observation_vector.size(); i++)
    {
        IntegrationVector interpolated_vector;
        for (int j = last; j < interpolation_orbits.size(); j++) 
        {
            if (observation_vector[i].get_date()->get_MJD() < interpolation_orbits[j].get_date().get_MJD())
            {
                last = j - 1;
                double f_current = interpolation_orbits[j].get_barycentric_position().get_alpha();
                double f_previous = interpolation_orbits[j - 1].get_barycentric_position().get_alpha();
                double t_current = interpolation_orbits[j].get_date().get_MJD();
                double t_previous = interpolation_orbits[j - 1].get_date().get_MJD();
                double t_interpolate = observation_vector[i].get_date()->get_MJD();
                interpolation_alpha_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                f_current = interpolation_orbits[j].get_barycentric_position().get_beta();
                f_previous = interpolation_orbits[j - 1].get_barycentric_position().get_beta();
                interpolation_beta_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                f_current = interpolation_orbits[j].get_barycentric_position().get_gamma();
                f_previous = interpolation_orbits[j - 1].get_barycentric_position().get_gamma();
                interpolation_gamma_term = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

                Date new_date = *observation_vector[i].get_date();
                interpolated_vector.set_date(new_date);
                interpolated_vector.set_barycentric_position(interpolation_alpha_term, interpolation_beta_term, interpolation_gamma_term);
                interpolated_vector.set_velocity(interpolation_orbits[j].get_velocity().get_vx(), interpolation_orbits[j].get_velocity().get_vy(), interpolation_orbits[j].get_velocity().get_vz());
                result.push_back(interpolated_vector);
                break;
            }
        }
    }
    return result;
}