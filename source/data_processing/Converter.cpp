#include "Converter.h"



Converter::Converter(Interpolator* interpolator)
{
    this->interpolator = interpolator;
}

void Converter::set_tdb_grid(std::vector<TT_TDB_obj>* tdb_grid)
{
    this->tdb_grid = tdb_grid;
}




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

    return cartesian_coordinates;
}



/*
    This method converts the date from UTC to TT via TAI.
    UTC <-> TAI <-> TT
*/
void Converter::UTC_to_TT(Date* date, Observation* observation)
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

        Date TT;
        date->set_TT(date->get_MJD() + (deltat + 32.184) / 86400); // TT in days
        double day_fraction = date->get_day_fraction() + (deltat + 32.184) / 86400;
        TT.set_MJD(date->get_TT());
        TT.set_day(date->get_day());
        TT.set_month(date->get_month());
        if (day_fraction >= 1.0)
        {
            TT.set_day(date->get_day() + (int)day_fraction);
            TT.set_day_fraction(date->get_day_fraction() - 1);
        }

        double h, m, s;
        // adding h, mn, s, ms
        h = (int)(day_fraction * 24);
        m = (day_fraction * 24.0 - (int)(day_fraction * 24)) * 60;
        s = ((day_fraction * 24.0 - (int)(day_fraction * 24)) * 60 - (int)((day_fraction * 24.0 - (int)(day_fraction * 24)) * 60)) * 60;

        TT.set_year(date->get_year());
        TT.set_hours(h);
        TT.set_minutes(m);
        TT.set_seconds(s);

        observation->set_TT(TT);
    }
}


/*
    Method for converting from cartesian to geocentric coordinates for observatories
*/
GeocentricCoord Converter::terrestial_to_geocentric_celestial(CartesianCoord position, Date date, Date TT, EarthRotation earth_rotation)
{
    double celestial_to_terestial[3][3];

    double uta, utb;

    double tt1, tt2;

    double ut1, ut2;
    /*  Given:
            utc1,utc2  double   UTC as a 2-part quasi Julian Date (Notes 1-4)
            dut1       double   Delta UT1 = UT1-UTC in seconds (Note 5)
        Returned:
            ut11,ut12  double   UT1 as a 2-part Julian Date (Note 6)
    */

    iauDtf2d("UTC", date.get_year(), date.get_month(), date.get_day(), date.get_hours(), date.get_minutes(), date.get_seconds(), &ut1, &ut2);
    iauUtcut1(ut1, ut2, earth_rotation.get_UT1_UTC(), &uta, &utb);

    iauDtf2d("TT", TT.get_year(), TT.get_month(), TT.get_day(), TT.get_hours(), TT.get_minutes(), TT.get_seconds(), &tt1, &tt2);

    iauC2t06a(tt1, tt2, uta, utb, earth_rotation.get_x() / 206265, earth_rotation.get_y() / 206265, celestial_to_terestial);

    /*
                   (x)
        (matrix) * (y)
                   (z)
    */
    double x = celestial_to_terestial[0][0] * position.get_x() + celestial_to_terestial[1][0] * position.get_y() + celestial_to_terestial[2][0] * position.get_z();
    double y = celestial_to_terestial[0][1] * position.get_x() + celestial_to_terestial[1][1] * position.get_y() + celestial_to_terestial[2][1] * position.get_z();
    double z = celestial_to_terestial[0][2] * position.get_x() + celestial_to_terestial[1][2] * position.get_y() + celestial_to_terestial[2][2] * position.get_z();

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
}



/*
    Convert spherical coordinates from "hours-minutes-seconds" system to degrees system (radians)
*/
void Converter::spherical_hours_to_spherical_radians(Observation* observation)
{
    // Convert from hours-system to degrees:
    // https://planetcalc.ru/7663/
    double* RA_in_hours_system = observation->get_spherical_position().get_RA_in_hours_system();
    double degrees = 15 * RA_in_hours_system[0];
    double arcminutes = 0.25 * RA_in_hours_system[1];
    double arcseconds = 0.25 * RA_in_hours_system[2];

    degrees = degrees + int(arcminutes);
    arcminutes = (arcminutes - int(arcminutes)) * 60 + int(arcseconds);
    arcseconds = (arcseconds - int(arcseconds)) * 60;

    double ascension;
    iauAf2a('+', degrees, arcminutes, arcseconds, &ascension);

    double* DEC_in_degrees_system = observation->get_spherical_position().get_DEC_in_degrees_system();
    char sign = DEC_in_degrees_system[0] < 0 ? '-' : '+';

    degrees = DEC_in_degrees_system[0];
    arcminutes = DEC_in_degrees_system[1];
    arcseconds = DEC_in_degrees_system[2];

    double declination;
    iauAf2a(sign, degrees, arcminutes, arcseconds, &declination);

    while ((ascension > PI) or (ascension < -PI))
    {
        int sign = ascension > PI ? -1 : 1;
        ascension = ascension + sign * 2 * PI;
    }

    observation->set_spherical(ascension, declination);
}


void Converter::barycentric_cartesian_to_geocentric_cartesian(ModelMeasure* model_measure, std::vector<IntegrationVector>* earth)
{
    BarycentricCoord earth_position = this->interpolator->find_object_position(model_measure->get_date(), earth);
    GeocentricCoord oumuamua_position;
    oumuamua_position.set_x(model_measure->get_barycentric().get_x() - earth_position.get_x());
    oumuamua_position.set_y(model_measure->get_barycentric().get_y() - earth_position.get_y());
    oumuamua_position.set_z(model_measure->get_barycentric().get_z() - earth_position.get_z());
    model_measure->set_geocentric(oumuamua_position);
}


void Converter::barycentric_cartesian_to_geocentric_cartesian(IntegrationVector* model_measure, std::vector<IntegrationVector>* earth)
{
    BarycentricCoord earth_position = this->interpolator->find_object_position(model_measure->get_date(), earth);
    BarycentricCoord oumuamua_position;
    oumuamua_position.set_x(model_measure->get_barycentric().get_x() - earth_position.get_x());
    oumuamua_position.set_y(model_measure->get_barycentric().get_y() - earth_position.get_y());
    oumuamua_position.set_z(model_measure->get_barycentric().get_z() - earth_position.get_z());
    model_measure->set_barycentric(oumuamua_position); // add set geocentric 
}


void Converter::geocentric_cartesian_to_geocentric_spherical(ModelMeasure* model_measure)
{
    double geocentric_coord[3] = { model_measure->get_geocentric().get_x(), model_measure->get_geocentric().get_y(), model_measure->get_geocentric().get_z() };
    double right_ascension;
    double declination;

    iauC2s(geocentric_coord, &right_ascension, &declination);

    while ((right_ascension > PI) or (right_ascension < -PI))
    {
        int sign = right_ascension > PI ? -1 : 1;
        right_ascension = right_ascension + sign * 2 * PI;
    }

    SphericalCoord temp_coords;
    temp_coords.set_spherical(right_ascension, declination);
    model_measure->set_spherical(temp_coords);
}


void Converter::barycentric_cartesian_to_barycentric_spherical(ModelMeasure* model_measure)
{
    double barycentric_coord[3] = { model_measure->get_barycentric().get_x(), model_measure->get_barycentric().get_y(), model_measure->get_barycentric().get_z() };
    double right_ascension;
    double declination;

    iauC2s(barycentric_coord, &right_ascension, &declination);
    SphericalCoord temp_coords;
    temp_coords.set_spherical(right_ascension, declination);
    model_measure->set_spherical(temp_coords);
}


/*
    Convert geocentric to barycentric coordinates
    Used for base observation
*/
void Converter::cartesian_geocentric_to_cartesian_barycentric(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<EarthRotation>* earth_rotation, std::vector<IntegrationVector>* hubble_data, std::vector<IntegrationVector>* earth_position)
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
            
            Date TT = *observations->at(i).get_TT();
            // convert observatory coordinates from cartesian to geocentric
            GeocentricCoord geocentric_observatory_position = terrestial_to_geocentric_celestial(current_observatory->get_cartesian(), *current_date, TT, earth_rotation_info);
            // interpolation observatory coordinates to Earth center
            // [barycentric position of the center of the Earth] + [celestial geocentric position of the observatory]
            
            //TT -> TDB
            double TDB = TT.get_MJD() - this->interpolator->interpolation_time(TT.get_MJD(), this->tdb_grid) / 86400000;
            observations->at(i).set_TDB(TDB);

            Date tdb_time;
            tdb_time.set_MJD(TDB);

            BarycentricCoord interpolated_Earth_center = interpolator->find_object_position(tdb_time, earth_position);
            observatory_position.set_all_coords(interpolated_Earth_center.get_x() + geocentric_observatory_position.get_x(), interpolated_Earth_center.get_y() + geocentric_observatory_position.get_y(), interpolated_Earth_center.get_z() + geocentric_observatory_position.get_z());
        }
        else
        {
            observations->at(i).set_TDB(current_date->get_MJD());

            BarycentricCoord hubble_position = interpolator->find_object_position(*current_date, hubble_data);
            observatory_position.set_all_coords(hubble_position.get_x(), hubble_position.get_y(), hubble_position.get_z());
        }

        observations->at(i).set_observatory_position(observatory_position);
    }
}

