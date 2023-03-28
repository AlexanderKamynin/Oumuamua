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
        x = rho * cos (phi) * cos (delta)
        y = rho * sin (phi) * cos (delta)
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
    Method for converting from cartesian to geocentric celestial coordinates for observatories
*/
GeocentricCoord Converter::cartesian_to_geocentric(CartesianCoord position, Date date, EarthRotation earth_rotation) 
{
    double geocentric_to_cartesian[3][3];
    
    // Input: We use JD method, so tta=TT, ttb=0, uta=MJD earth rotation, utb=0, xp=0, yp=0 -> return matrix for converting
    iauC2t06a(date.get_TT(), 0, date.get_JD(), 0, earth_rotation.get_x(), earth_rotation.get_y(), geocentric_to_cartesian);

    // transpose for convert from cartesian to geocentric
    this->help.transpose_matrix(geocentric_to_cartesian);

    /*
        (x)
        (y) * (matrix)
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
    Interpolation of Earth center coordinates and calculation of observatory coordinates in the inertial system
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
            interpolated_position.set_alpha(obsevatory_position.get_x() + interpolation_alpha_term);
            interpolated_position.set_beta(obsevatory_position.get_y() + interpolation_beta_term);
            interpolated_position.set_gamma(obsevatory_position.get_z() + interpolation_gamma_term);
            return interpolated_position;
        }
    }
}


/*
    Interpolation of all center planets coordinates
*/
std::map<std::string, std::vector<IntegrationVector>> Converter::interpolation_center_planet(double h, Date* date_start, Date* date_finish, std::map<std::string, std::vector<IntegrationVector>> interpolation_planets)
{
    double delta_x;
    double delta_y;
    double delta_z;
    std::map<std::string, std::vector<IntegrationVector>> new_interpolation_planet;
    for (auto interpolation_planet : interpolation_planets)
    {
        int last_min = 0;
        double flag = 1;
        Date current_date = *date_start;
        int count = 0;
        std::vector<IntegrationVector> new_center_planet;
        while (current_date.get_MJD() < date_finish->get_MJD()) 
        {
            int j = last_min;
            for (j; j < interpolation_planet.second.size(); j++) 
            {
                //@change get_date -> get_date
                if (current_date.get_MJD() < (interpolation_planet.second[j].get_date()).get_MJD())
                {

                    last_min = j - 1;
                    //@change get_date -> get_date
                    if (current_date.get_MJD() == (interpolation_planet.second[last_min].get_date()).get_MJD()) // проверить какая дата у current_date get+MJD/TDB
                        flag = 0;
                    IntegrationVector new_vector;

                    //@change x -> alpha; get_position -> get_barycentric_position; get_date -> get_date
                    delta_x = interpolation_planet.second[last_min].get_barycentric_position().get_alpha() + flag * (interpolation_planet.second[j].get_barycentric_position().get_alpha() - interpolation_planet.second[last_min].get_barycentric_position().get_alpha()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    //@change y -> beta; get_position -> get_barycentric_position; get_date -> get_date
                    delta_y = interpolation_planet.second[last_min].get_barycentric_position().get_beta() + flag * (interpolation_planet.second[j].get_barycentric_position().get_beta() - interpolation_planet.second[last_min].get_barycentric_position().get_beta()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    //@change z -> gamma; get_position -> get_barycentric_position; get_date -> get_date
                    delta_z = interpolation_planet.second[last_min].get_barycentric_position().get_gamma() + flag * (interpolation_planet.second[j].get_barycentric_position().get_gamma() - interpolation_planet.second[last_min].get_barycentric_position().get_gamma()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    flag = 1;
                    //@change set_date -> set_date
                    new_vector.set_date(current_date);
                    //@change set_position -> set_barycentric_position
                    new_vector.set_barycentric_position(delta_x, delta_y, delta_z);
                    new_center_planet.push_back(new_vector);
                    current_date.set_MJD(current_date.get_MJD() + h);

                    break;
                }
            }
        }
        new_interpolation_planet[interpolation_planet.first] = new_center_planet;
    }
    return new_interpolation_planet;
}


/*
    This method convert spherical coordinates to geocentric
*/
void Converter::spherical_to_geocentric(Observation* observation)
{
    double cart_coord[3];
    iauS2c(observation->get_spherical_position().get_longitude(), observation->get_spherical_position().get_latitude(), cart_coord);
    observation->set_geocentric(cart_coord[0] * 6378.140, cart_coord[1] * 6378.140, cart_coord[2] * 6378.140);
}


// Convert celestial coordinates from "hours-minutes-seconds" system to degrees system and then to radians
void Converter::celestial_to_spherical(Observation* observation) 
{
    // Convert from hours-system to degrees:
    // https://planetcalc.ru/7663/
    double degrees = 15 * observation->get_ascension().get_hours();
    double arcminutes = 0.25 * observation->get_ascension().get_minutes();
    double arcseconds = 0.25 * observation->get_ascension().get_seconds();
    char sign;
    if (observation->get_declination().get_hours() < 0)
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


    if (observation->get_declination().get_hours() < 0) 
    {
        sign = '-';
    }
    else
    {
        sign = '+';
    }
    
    degrees = 15 * observation->get_declination().get_hours();
    arcminutes = 0.25 * observation->get_declination().get_minutes();
    arcseconds = 0.25 * observation->get_declination().get_seconds();

    arcminutes += int(arcseconds) / 60; // if arcseconds >= 60 then add to arcminutes
    arcseconds = arcseconds - int(arcseconds) / 60;

    degrees += int(arcminutes) / 60; // if arcminutes >= 60 then add to degrees
    arcminutes = arcminutes - int(arcminutes) / 60;

    double declination;
    iauAf2a(sign, degrees, arcminutes, arcseconds, &declination);

    observation->set_spherical(ascension, declination);
}


//Перевод барицентрических координат в сферические
void Converter::barycentric_to_spherical(IntegrationVector* vector) 
{
    //@change x, y, z -> alpha, beta, gamma; get_position -> get_barycentric_position
    double bary[3] = { vector->get_barycentric_position().get_alpha(), vector->get_barycentric_position().get_beta(), vector->get_barycentric_position().get_gamma() };
    double longitude;
    double latitude;
    iauC2s(bary, &longitude, &latitude);
    vector->set_spherical_position(longitude, latitude);
}


//Перевод барицентрических координат в сферические
void Converter::barycentric_to_spherical_for_observations(Observation* vector)
{
    //@change x, y, z -> alpha, beta, gamma
    double bary[3] = { vector->get_barycentric().get_alpha(), vector->get_barycentric().get_beta(), vector->get_barycentric().get_gamma() };
    double longitude;
    double latitude;
    iauC2s(bary, &longitude, &latitude);
    vector->set_spherical(longitude, latitude);
}


void Converter::geocentric_to_barycentric(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<EarthRotation>* earth_rotation, std::vector<HubbleData> hubble_map, std::vector<IntegrationVector> earth_position) 
{
    double ind = 0;
    for (int i = 0; i < observations->size(); i++)
    {
        BarycentricCoord observation_position;

        BarycentricCoord observatory_position;
        if (observations->at(i).get_code() != "250")
        {
            std::string code = observations->at(i).get_code();
            Observatory cur_obs = observatory->at(code);

            // find earth rotation info for current date observation
            EarthRotation earth_rotation_info;
            for (int j = 0; j < earth_rotation->size(); j++) {
                if (earth_rotation->at(j).get_MJD() >= observations->at(i).get_date()->get_MJD()) {
                    earth_rotation_info = earth_rotation->at(j);
                    break;
                }
            }
            GeocentricCoord geo_obs = cartesian_to_geocentric(cur_obs.get_cartesian(), *observations->at(i).get_date(), earth_rotation_info);
            observatory_position = interpolation_Earth_center(*observations->at(i).get_date(), geo_obs, earth_position);
        }
        else
        {
            GeocentricCoord tmp_position = find_needed_hubble_data(*observations->at(i).get_date(), hubble_map);
            BarycentricCoord HM = interpolation_Earth_center(*observations->at(i).get_date(), tmp_position, earth_position);
            observatory_position = HM;
        }
        //@change x -> alpha
        observation_position.set_alpha(observatory_position.get_alpha() + observations->at(i).get_geocentric().get_x());
        //@change y -> beta
        observation_position.set_beta(observatory_position.get_beta() + observations->at(i).get_geocentric().get_y());
        //@change z -> gamma
        observation_position.set_gamma(observatory_position.get_gamma() + observations->at(i).get_geocentric().get_z());
        //@change x, y, z -> alpha, beta, gamma
        observations->at(i).set_barycentric(observation_position.get_alpha(), observation_position.get_beta(), observation_position.get_gamma());

    }
}


//Интерполяция для выявления смоделированных значений с нужными датами
std::vector<IntegrationVector> Converter::interpolation_to_observation(std::vector<Observation> vector, std::vector<IntegrationVector> interpolation_orbits) 
{
    double delta_x;
    double delta_y;
    double delta_z;
    int last_min = 0;
    std::vector<IntegrationVector> result;
    for (int i = 0; i < vector.size(); i++)
    {
        IntegrationVector new_vector;
        int j = last_min;
        int count = 0;
        for (j; j < interpolation_orbits.size(); j++) 
        {
            count++;
            //@change get_date -> get_date
            if (vector[i].get_date()->get_MJD() < interpolation_orbits[j].get_date().get_MJD()) 
            {
                last_min = j - 1;
                double t;
                //@change x -> alpha; get_position -> get_barycentric_position; get_date -> get_date
                delta_x = interpolation_orbits[j - 1].get_barycentric_position().get_alpha() + (interpolation_orbits[j].get_barycentric_position().get_alpha() - interpolation_orbits[j - 1].get_barycentric_position().get_alpha()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                //@change y -> beta; get_position -> get_barycentric_position; get_date -> get_date
                delta_y = interpolation_orbits[j - 1].get_barycentric_position().get_beta() + (interpolation_orbits[j].get_barycentric_position().get_beta() - interpolation_orbits[j - 1].get_barycentric_position().get_beta()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                //@change z -> gamma; get_position -> get_barycentric_position; get_date -> get_date
                delta_z = interpolation_orbits[j - 1].get_barycentric_position().get_gamma() + (interpolation_orbits[j].get_barycentric_position().get_gamma() - interpolation_orbits[j - 1].get_barycentric_position().get_gamma()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                Date new_data = *vector[i].get_date();
                //@change set_date -> set_date
                new_vector.set_date(new_data);
                //@change set_position -> set_barycentric_position
                new_vector.set_barycentric_position(delta_x, delta_y, delta_z);
                new_vector.set_velocity(0, 0, 0);
                result.push_back(new_vector);
                break;
            }
        }
    }
    return result;
}


//Нахождение координат по измененной дате (для поправок за световой промежуток)
BarycentricCoord Converter::interpolation_orbits(double date, std::vector<Observation> interpolation_orbits) 
{
    double delta_x;
    double delta_y;
    double delta_z;
    for (int j = 0; j < interpolation_orbits.size(); j++) 
    {
        if (date < (*interpolation_orbits[j].get_date()).get_MJD()) 
        {
            //@change get_x -> alpha
            delta_x = interpolation_orbits[j - 1].get_barycentric().get_alpha() + (interpolation_orbits[j].get_barycentric().get_alpha() - interpolation_orbits[j - 1].get_barycentric().get_alpha()) / ((*interpolation_orbits[j].get_date()).get_MJD() - (*interpolation_orbits[j - 1].get_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_date()).get_MJD());
            //@change y -> betta
            delta_y = interpolation_orbits[j - 1].get_barycentric().get_beta() + (interpolation_orbits[j].get_barycentric().get_beta() - interpolation_orbits[j - 1].get_barycentric().get_beta()) / ((*interpolation_orbits[j].get_date()).get_MJD() - (*interpolation_orbits[j - 1].get_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_date()).get_MJD());
            //@change z -> gamma
            delta_z = interpolation_orbits[j - 1].get_barycentric().get_gamma() + (interpolation_orbits[j].get_barycentric().get_gamma() - interpolation_orbits[j - 1].get_barycentric().get_gamma()) / ((*interpolation_orbits[j].get_date()).get_MJD() - (*interpolation_orbits[j - 1].get_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_date()).get_MJD());
            BarycentricCoord new_frame;
            //@change x -> alpha
            new_frame.set_alpha(delta_x);
            //@change y -> beta
            new_frame.set_beta(delta_y);
            //@change z -> gamma
            new_frame.set_gamma(delta_z);
            return new_frame;
        }
    }
};
