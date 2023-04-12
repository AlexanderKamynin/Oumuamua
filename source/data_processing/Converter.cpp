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
    
    // convert degrees to radians  y degrees = y * pi / 180 radians
    double angle = coordinates.get_longitude() * PI / 180;

    cartesian_coordinates.set_x(std::cos(angle) * coordinates.get_cos() * EARTH_RADIUS);
    cartesian_coordinates.set_y(std::sin(angle) * coordinates.get_cos() * EARTH_RADIUS);
    cartesian_coordinates.set_z(coordinates.get_sin() * EARTH_RADIUS);

    //@CHECK
    //std::cout << "cylindrical was: "
    //    << "longi=" << coordinates.get_longitude()
    //    << " cos=" << coordinates.get_cos()
    //    << " sin=" << coordinates.get_sin() << "\n";
    
    //std::cout << "cartesian got: "
    //    << "x=" << cartesian_coordinates.get_x()
    //    << " y=" << cartesian_coordinates.get_y()
    //    << " z=" << cartesian_coordinates.get_z() << "\n\n";

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

        //@CHECK
        //std::cout << "Year=" << date->get_year() << " month=" << date->get_month() << " day=" << date->get_day() << " seconds=" << date->get_seconds() << " day_fraction=" << date->get_day_fraction() <<
        //    " deltat=" << deltat << " s." << " MJD=" << date->get_MJD() << " TT=" << date->get_TT() << "\n\n";
    }
}



/*
    Interpolation of time on a uniform grid for observations
*/
void Converter::interpolation_time(Date* date_start, std::vector<Observation>* observations, std::vector<InterpolationTime> time)
{
    double interpolation_time_term;
    int last = 0;
    for (int i = 0; i < observations->size(); i++)
    {
        int expected_j = 10 * int(observations->at(i).get_date()->get_day_fraction() * 10) + (int(observations->at(i).get_date()->get_MJD()) - int(time[0].get_date().get_MJD())) * 100 + 5; // constatnt find j
        if (not (observations->at(i).get_date()->get_MJD() < time[expected_j].get_date().get_MJD() and observations->at(i).get_date()->get_MJD() > time[expected_j - 1].get_date().get_MJD())) //checking j
        {
            expected_j += 1;
        }
        double f_current = time[expected_j].get_TT_TDB();
        double f_previous = time[expected_j - 1].get_TT_TDB();
        double t_current = time[expected_j].get_date().get_MJD();
        double t_previous = time[expected_j - 1].get_date().get_MJD();
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
GeocentricCoord Converter::terrestial_to_geocentric_celestial(CartesianCoord position, Date date, EarthRotation earth_rotation)
{
    double celestial_to_terestial[3][3];

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
    iauC2t06a(date.get_TT(), 2400000.5, uta, utb, earth_rotation.get_x(), earth_rotation.get_y(), celestial_to_terestial);

    // transpose for convert from cartesian to geocentric
    this->help.transpose_matrix(celestial_to_terestial);

    /*
                   (x)
        (matrix) * (y)
                   (z)
    */
    double x = celestial_to_terestial[0][0] * position.get_x() + celestial_to_terestial[0][1] * position.get_y() + celestial_to_terestial[0][2] * position.get_z();
    double y = celestial_to_terestial[1][0] * position.get_x() + celestial_to_terestial[1][1] * position.get_y() + celestial_to_terestial[1][2] * position.get_z();
    double z = celestial_to_terestial[2][0] * position.get_x() + celestial_to_terestial[2][1] * position.get_y() + celestial_to_terestial[2][2] * position.get_z();

    GeocentricCoord geocentric_position;
    geocentric_position.set_x(x);
    geocentric_position.set_y(y);
    geocentric_position.set_z(z);

    return geocentric_position;
};


/*
    Сonvert spherical coordinates to geocentric
*/
void Converter::barycentric_spherical_to_geocentric_cartesian(Observation* observation)
{
    double cartesian_coord[3];
    iauS2c(observation->get_spherical_position().get_right_ascension(), observation->get_spherical_position().get_declination(), cartesian_coord);

    observation->set_geocentric(cartesian_coord[0] * EARTH_RADIUS, cartesian_coord[1] * EARTH_RADIUS, cartesian_coord[2] * EARTH_RADIUS);

    //@CHECK
    /*std::cout << "observation in spherical: " << "RA=" << observation->get_spherical_position().get_right_ascension() << " DEC=" << observation->get_spherical_position().get_declination() << "\n";
    std::cout << "observation in geocentric: " 
        << "x=" << observation->get_geocentric().get_x() 
        << " y=" << observation->get_geocentric().get_y()
        << " z=" << observation->get_geocentric().get_z() << "\n\n";*/
}



/*
    Convert spherical coordinates from "hours-minutes-seconds" system to degrees system (radians)
*/
void Converter::spherical_hours_to_spherical_radians(Observation* observation)
{
    //@CHECK
    //std::cout << "observation: " << "MJD=" << observation->get_date()->get_MJD() << "\nRA in hours system="
    //    << observation->get_spherical_position().get_RA_in_hours_system()[0] << " "
    //    << observation->get_spherical_position().get_RA_in_hours_system()[1] << " "
    //    << observation->get_spherical_position().get_RA_in_hours_system()[2] << " "
    //    << "\nDEC in hours system="
    //    << observation->get_spherical_position().get_DEC_in_hours_system()[0] << " "
    //    << observation->get_spherical_position().get_DEC_in_hours_system()[1] << " "
    //    << observation->get_spherical_position().get_DEC_in_hours_system()[2] << "\n";


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
        degrees *= -1;
    }
    else
    {
        sign = '+';
    }

    degrees = degrees + int(arcminutes);
    arcminutes = (arcminutes - int(arcminutes)) * 60 + int(arcseconds);
    arcseconds = (arcseconds - int(arcseconds)) * 60;

    double ascension;
    //@CHECK
    /*std::cout << "before ASCENTION: "
        << " sign=" << sign
        << " degrees=" << degrees
        << " arcminutes=" << arcminutes
        << " arcseconds=" << arcseconds << "\n";*/
    iauAf2a('+', degrees, arcminutes, arcseconds, &ascension);

    double* DEC_in_hours_system = observation->get_spherical_position().get_DEC_in_hours_system();

    if (DEC_in_hours_system[0] < 0)
    {
        sign = '-';
        degrees = 15 * DEC_in_hours_system[0] * (-1);
    }
    else
    {
        degrees = 15 * DEC_in_hours_system[0];
        sign = '+';
    }

    arcminutes = 0.25 * DEC_in_hours_system[1];
    arcseconds = 0.25 * DEC_in_hours_system[2];

    degrees = degrees + int(arcminutes);
    arcminutes = (arcminutes - int(arcminutes)) * 60 + int(arcseconds);
    arcseconds = (arcseconds - int(arcseconds)) * 60;

    double declination;
    //@CHECK
   /* std::cout << "before DECLINATION: "
        << " sign=" << sign
        << " degrees=" << degrees
        << " arcminutes=" << arcminutes
        << " arcseconds=" << arcseconds << "\n";*/
    iauAf2a(sign, degrees, arcminutes, arcseconds, &declination);

    while ((ascension > PI) or (ascension < - PI)) {
        int sign = ascension > PI ? -1 : 1;
        ascension = ascension + sign * 2 * PI;
    }

    observation->set_spherical(ascension, declination);

    //@CHECK
    // std::cout << "RESULT: RA in radians=" << ascension << " DEC in radians=" << declination << "\n\n";
}



/*
    Convert from barycentric to spherical coodinates
    Method for IntegrationVector
*/
void Converter::barycentric_cartesian_to_barycentric_spherical(IntegrationVector* vector, std::vector<SphericalCoord>* coords)
{
    //std::cout << "Before convertation:" << vector->get_barycentric_position().get_alpha() << "|" << vector->get_barycentric_position().get_beta() << "|" << vector->get_barycentric_position().get_gamma() << std::endl;
    double barycentric_coord[3] = { vector->get_barycentric_position().get_alpha(), vector->get_barycentric_position().get_beta(), vector->get_barycentric_position().get_gamma() };
    double right_ascension;
    double declination;

    iauC2s(barycentric_coord, &right_ascension, &declination);
    //std::cout << "After convertation:" << right_ascension << "|" << declination << std::endl;
    SphericalCoord temp_coords;
    temp_coords.set_declination(declination);
    temp_coords.set_right_ascension(right_ascension);
    coords->push_back(temp_coords);
}


/*
    Convert geocentric to barycentric coordinates
    Used for base observation
*/
void Converter::cartesian_geocentric_to_cartesian_barycentric(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<EarthRotation>* earth_rotation, std::vector<HubbleData> hubble_data, std::vector<IntegrationVector> earth_position)
{
    Date* start_date = observations->at(0).get_date();
    for (int i = 0; i < observations->size(); i++)
    {
        Observatory* current_observatory = &observatory->at(observations->at(i).get_code());
        GeocentricCoord Oumuamua_position = observations->at(i).get_geocentric();
        BarycentricCoord observatory_position;
        Date* current_date = observations->at(i).get_date();

        if (observations->at(i).get_code() != "250") // 250 - Hubble code
        {
            // find earth rotation info for current date observation
            EarthRotation earth_rotation_info;
            for (int j = 0; j < earth_rotation->size(); j++) 
            {
                if (earth_rotation->at(j).get_MJD() >= current_date->get_MJD()) 
                {
                    earth_rotation_info = earth_rotation->at(j);
                    break;
                }
            }

            // convert observatory coordinates from cartesian to geocentric
            GeocentricCoord geocentric_observatory_position = terrestial_to_geocentric_celestial(current_observatory->get_cartesian(), *current_date, earth_rotation_info);
            // interpolation observatory coordinates to Earth center
            // [barycentric position of the center of the Earth] + [celestial geocentric position of the observatory]
            BarycentricCoord interpolated_Earth_center = interpolation_Earth_center(*current_date, *start_date, earth_position);
            observatory_position.set_alpha(interpolated_Earth_center.get_alpha() + geocentric_observatory_position.get_x());
            observatory_position.set_beta(interpolated_Earth_center.get_beta() + geocentric_observatory_position.get_y());
            observatory_position.set_gamma(interpolated_Earth_center.get_gamma() + geocentric_observatory_position.get_z());
        }
        else
        {
            GeocentricCoord geocentric_hubble_position = find_needed_hubble_data(*current_date, hubble_data);
            // [barycentric position of the center of the Earth] + [celestial geocentric position of the observatory]
            BarycentricCoord interpolated_Earth_center = interpolation_Earth_center(*current_date, *start_date, earth_position);
            observatory_position.set_alpha(interpolated_Earth_center.get_alpha() + geocentric_hubble_position.get_x());
            observatory_position.set_beta(interpolated_Earth_center.get_beta() + geocentric_hubble_position.get_y());
            observatory_position.set_gamma(interpolated_Earth_center.get_gamma() + geocentric_hubble_position.get_z());
        }

        // set position for observatory
        current_observatory->set_barycentric(observatory_position);
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
        int j = int((observation_vector[i].get_date()->get_MJD() - date_start->get_MJD()) / STEP) + 2;
        BarycentricCoord interpolated_position = interpolation_helper(interpolation_orbits[j], interpolation_orbits[j - 1], *observation_vector[i].get_date());
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
                    last = j - 1;
                    BarycentricCoord interpolated_position_1 = interpolation_helper(interpolation_planet.second[j], interpolation_planet.second[j - 1], current_date);
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
    int i = int((date_current.get_MJD() - date_start.get_MJD()) / STEP) + 1;
    BarycentricCoord interpolated_position = interpolation_helper(earth_position[i], earth_position[i - 1], date_current);
    return interpolated_position;
}

BarycentricCoord Converter::interpolation_helper(IntegrationVector position_current, IntegrationVector position_previous, Date date)
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