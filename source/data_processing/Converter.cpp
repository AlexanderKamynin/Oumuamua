#include "Converter.h"

//Функция, переводащая цилиндрические координаты в декартовы
CartesianCoord Converter::cylindrical_to_cartesian(CylindricalCoord measure)
{
    CartesianCoord cartesian_data;
    if (measure.get_longitude() != NULL) 
    {
        if (measure.get_cos() != NULL) 
        {
            cartesian_data.set_x(cos(measure.get_longitude()) * measure.get_cos() * EARTH_RADIUS); // cartesian_data.set_x(cos(measure.get_longitude()) * 6378.140);
            cartesian_data.set_y(sin(measure.get_longitude()) * measure.get_cos() * EARTH_RADIUS); //cartesian_data.set_y(sin(measure.get_longitude()) * 6378.140);
        }
        if (measure.get_sin() != NULL)
        {
            cartesian_data.set_z(measure.get_sin() * EARTH_RADIUS);
        }
    }
    return cartesian_data;
}


//Gереводит в TT
void Converter::julian_date_to_tt(Date* date) 
{
    //Перевод в TAI
    double deltat;
    int status = iauDat(date->get_year(), date->get_month(), date->get_day(), date->get_day_fraction(),
        &deltat);

    if (status)
    {
        std::cout << "Что-то пошло не так\n";
        return;
    }
    //Перевод в TT
    date->set_TT(date->get_MJD() + deltat + 32.184);
};

//Интерполяция времени в TDB
void Converter::interpolation_date_to_tt_tdb(std::vector<Observation>* observations, std::vector<InterpolationTimeFrame> interpolation_time) 
{
    double delta;
    int last_min = 0;
    for (int i = 0; i < observations->size(); i++) 
    {
        int j = last_min;
        for (j; j < interpolation_time.size(); j++) 
        {
            if ((observations->at(i)).get_julian_date()->get_MJD() < interpolation_time[j].get_julian_date().get_MJD()) 
            {
                last_min = j - 1;
                delta = interpolation_time[j - 1].get_TT_TDB() + (interpolation_time[j].get_TT_TDB() - interpolation_time[j - 1].get_TT_TDB()) / (interpolation_time[j].get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD()) * ((observations->at(i)).get_julian_date()->get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD());
                (observations->at(i)).get_julian_date()->set_TT_TDB(delta);
                break;
            }
        }
    }
}

//Интерполяция времени в TDB
void Converter::interpolation_to_tdb(Date* date, std::vector<InterpolationTimeFrame> interpolation_time)
{
    double delta;

    for (int j = 0; j < interpolation_time.size(); j++) 
    {
        if (date->get_MJD() < interpolation_time[j].get_julian_date().get_MJD())
        {
            delta = interpolation_time[j - 1].get_TT_TDB() + (interpolation_time[j].get_TT_TDB() - interpolation_time[j - 1].get_TT_TDB()) / (interpolation_time[j].get_julian_date().get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD()) * (date->get_MJD() - interpolation_time[j - 1].get_julian_date().get_MJD());
            date->set_TT_TDB(delta);
            break;
        }
    }
}

//Интерполяция положения Хаббла
GeocentricCoord Converter::interpolation_hubble_data(Date date, std::vector<InterpolationHubbleFrame> interpolation_data)
{
    for (int i = 0; i < interpolation_data.size() - 1; i++)
    {
        if ((interpolation_data[i].get_date() < date) and (interpolation_data[i + 1].get_date() > date)) 
        {
            GeocentricCoord new_frame;

            double x = interpolation_data[i].get_geocentric().get_x() + (interpolation_data[i + 1].get_geocentric().get_x()
                - interpolation_data[i + 1].get_geocentric().get_x()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            double y = interpolation_data[i].get_geocentric().get_y() + (interpolation_data[i + 1].get_geocentric().get_y()
                - interpolation_data[i + 1].get_geocentric().get_y()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            double z = interpolation_data[i].get_geocentric().get_z() + (interpolation_data[i + 1].get_geocentric().get_z()
                - interpolation_data[i + 1].get_geocentric().get_z()) * (date.get_MJD() - interpolation_data[i].get_date().get_MJD());

            new_frame.set_x(x);
            new_frame.set_y(y);
            new_frame.set_z(z);
            return new_frame;
        }
    }
};


//Транспонирование матрицы 3х3
void Converter::transpose(double mtr[3][3]) 
{
    double tmp[3][3];
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++)
        {
            tmp[i][j] = mtr[i][j];
        }
    }
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++) 
        {
            mtr[j][i] = tmp[i][j];
        }
    }
}

//Перевод из земных координат в геоцентрические небесные
GeocentricCoord Converter::cartesian_to_geocentric(CartesianCoord frame, Date date) 
{
    double transition_matrix[3][3];

    double x, y;
    iauXy06(date.get_TT(), 0, &x, &y);

    iauC2t06a(date.get_TT(), 0, date.get_JD(), 0, x, y, transition_matrix);

    transpose(transition_matrix);

    double geo_x = transition_matrix[0][0] * frame.get_x() + transition_matrix[0][1] * frame.get_y() + transition_matrix[0][2] * frame.get_z();
    double geo_y = transition_matrix[1][0] * frame.get_x() + transition_matrix[1][1] * frame.get_y() + transition_matrix[1][2] * frame.get_z();
    double geo_z = transition_matrix[2][0] * frame.get_x() + transition_matrix[2][1] * frame.get_y() + transition_matrix[2][2] * frame.get_z();

    GeocentricCoord new_frame;
    new_frame.set_x(geo_x);
    new_frame.set_y(geo_y);
    new_frame.set_z(geo_z);
    return new_frame;

};

//Интерполяция центра Земли
BarycentricCoord Converter::interpolation_center_of_earth_for_observatory(Date date, GeocentricCoord frame, std::vector<IntegrationVector> interpolation_earth)
{
    double delta_x;
    double delta_y;
    double delta_z;
    for (int j = 0; j < interpolation_earth.size(); j++)
    {
        //@change get_julian_date -> get_date
        if (date.get_MJD() < interpolation_earth[j].get_date().get_MJD())
        {
            //@change get_x -> get_alpha; get_position -> get_barycentric_position; get_julian_date -> get_date
            delta_x = interpolation_earth[j - 1].get_barycentric_position().get_alpha() + (interpolation_earth[j].get_barycentric_position().get_alpha() - interpolation_earth[j - 1].get_barycentric_position().get_alpha()) / ((interpolation_earth[j].get_date()).get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD()) * (date.get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD());
            //@cahnge get_y -> get_beta; get_position -> get_barycentric_position; get_julian_date -> get_date
            delta_y = interpolation_earth[j - 1].get_barycentric_position().get_beta() + (interpolation_earth[j].get_barycentric_position().get_beta() - interpolation_earth[j - 1].get_barycentric_position().get_beta()) / ((interpolation_earth[j].get_date()).get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD()) * (date.get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD());
            //@change get_z -> get_gamma; get_position -> get_barycentric_position; get_julian_date -> get_date
            delta_z = interpolation_earth[j - 1].get_barycentric_position().get_gamma() + (interpolation_earth[j].get_barycentric_position().get_gamma() - interpolation_earth[j - 1].get_barycentric_position().get_gamma()) / ((interpolation_earth[j].get_date()).get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD()) * (date.get_MJD() - (interpolation_earth[j - 1].get_date()).get_MJD());
            BarycentricCoord new_frame;
            //@change set_x -> set_alpha
            new_frame.set_alpha(frame.get_x() + delta_x);
            //@change set_y -> set_beta
            new_frame.set_beta(frame.get_y() + delta_y);
            //@change set_z -> set_gamma
            new_frame.set_gamma(frame.get_z() + delta_z);
            return new_frame;

        }
    }
}

//Интерполяция координат центра Земли для численного интегрирования
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
                //@change get_julian_date -> get_date
                if (current_date.get_MJD() < (interpolation_planet.second[j].get_date()).get_MJD())
                {

                    last_min = j - 1;
                    //@change get_julian_date -> get_date
                    if (current_date.get_MJD() == (interpolation_planet.second[last_min].get_date()).get_MJD()) // проверить какая дата у current_date get+MJD/TDB
                        flag = 0;
                    IntegrationVector new_vector;

                    //@change x -> alpha; get_position -> get_barycentric_position; get_julian_date -> get_date
                    delta_x = interpolation_planet.second[last_min].get_barycentric_position().get_alpha() + flag * (interpolation_planet.second[j].get_barycentric_position().get_alpha() - interpolation_planet.second[last_min].get_barycentric_position().get_alpha()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    //@change y -> beta; get_position -> get_barycentric_position; get_julian_date -> get_date
                    delta_y = interpolation_planet.second[last_min].get_barycentric_position().get_beta() + flag * (interpolation_planet.second[j].get_barycentric_position().get_beta() - interpolation_planet.second[last_min].get_barycentric_position().get_beta()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    //@change z -> gamma; get_position -> get_barycentric_position; get_julian_date -> get_date
                    delta_z = interpolation_planet.second[last_min].get_barycentric_position().get_gamma() + flag * (interpolation_planet.second[j].get_barycentric_position().get_gamma() - interpolation_planet.second[last_min].get_barycentric_position().get_gamma()) / (interpolation_planet.second[j].get_date()).get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD() * (current_date.get_MJD() - (interpolation_planet.second[last_min].get_date()).get_MJD());
                    flag = 1;
                    //@change set_julian_date -> set_date
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

//Перевод сферических координат из "часы-минуты-секунды" в радианы
void Converter::celestial_to_spherical(Observation* observation) 
{
    double degrees_AD = 15.0 * observation->get_ascension().get_h();
    double arcminutes_AD = 0.25 * observation->get_ascension().get_m();
    double arcseconds_AD = 0.25 * observation->get_ascension().get_s();
    double delta_min = (int)arcminutes_AD;
    double delta_sec = (int)arcseconds_AD;
    degrees_AD += delta_min;
    arcminutes_AD = (arcminutes_AD - delta_min) * 60 + delta_sec;
    arcseconds_AD = (arcseconds_AD - delta_sec) * 60;
    double AD = NULL;
    double DEC = NULL;
    iauAf2a('+', degrees_AD, arcminutes_AD, arcseconds_AD, &AD);
    iauAf2a('+', observation->get_declination().get_h(), observation->get_declination().get_m(), observation->get_declination().get_s(), &DEC);
    observation->set_spherical(AD, DEC);
}

void Converter::spherical_to_geocentric(Observation* observation) 
{
    double cart_coord[3];
    iauS2c(observation->get_spherical_position().get_longitude(), observation->get_spherical_position().get_latitude(), cart_coord);
    observation->set_geocentric(cart_coord[0] * 6378.140, cart_coord[1] * 6378.140, cart_coord[2] * 6378.140);
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

void Converter::geocentric_to_barycentric(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<InterpolationHubbleFrame> hubble_map, std::vector<IntegrationVector> earth_position) 
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
            GeocentricCoord geo_obs = cartesian_to_geocentric(cur_obs.get_cartesian(), *observations->at(i).get_julian_date());
            observatory_position = interpolation_center_of_earth_for_observatory(*observations->at(i).get_julian_date(), geo_obs, earth_position);
        }
        else
        {
            GeocentricCoord tmp_position = interpolation_hubble_data(*observations->at(i).get_julian_date(), hubble_map);
            BarycentricCoord HM = interpolation_center_of_earth_for_observatory(*observations->at(i).get_julian_date(), tmp_position, earth_position);
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
            //@change get_julian_date -> get_date
            if (vector[i].get_julian_date()->get_MJD() < interpolation_orbits[j].get_date().get_MJD()) 
            {
                last_min = j - 1;
                double t;
                //@change x -> alpha; get_position -> get_barycentric_position; get_julian_date -> get_date
                delta_x = interpolation_orbits[j - 1].get_barycentric_position().get_alpha() + (interpolation_orbits[j].get_barycentric_position().get_alpha() - interpolation_orbits[j - 1].get_barycentric_position().get_alpha()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_julian_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                //@change y -> beta; get_position -> get_barycentric_position; get_julian_date -> get_date
                delta_y = interpolation_orbits[j - 1].get_barycentric_position().get_beta() + (interpolation_orbits[j].get_barycentric_position().get_beta() - interpolation_orbits[j - 1].get_barycentric_position().get_beta()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_julian_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                //@change z -> gamma; get_position -> get_barycentric_position; get_julian_date -> get_date
                delta_z = interpolation_orbits[j - 1].get_barycentric_position().get_gamma() + (interpolation_orbits[j].get_barycentric_position().get_gamma() - interpolation_orbits[j - 1].get_barycentric_position().get_gamma()) / ((interpolation_orbits[j].get_date()).get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD()) * (vector[i].get_julian_date()->get_MJD() - (interpolation_orbits[j - 1].get_date()).get_MJD());
                Date new_data = *vector[i].get_julian_date();
                //@change set_julian_date -> set_date
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

//Поправка за световой промежуток
std::vector<IntegrationVector> Converter::light_time_correction(std::map<std::string, Observatory> observatory, std::vector< Observation> observations, std::vector<InterpolationHubbleFrame> hubble_map, std::vector<IntegrationVector> earth_position) 
{
    std::vector<IntegrationVector> result;
    for (int i = 0; i < observations.size(); i++) 
    {
        IntegrationVector new_vector;
        double delta_t = (*observations[i].get_julian_date()).get_MJD();
        double delta = 0;
        for (int j = 0; j < 3; j++) {
            BarycentricCoord observatory_position;
            if (observations[i].get_code() != "250")
            {
                std::string code = observations[i].get_code();
                Observatory cur_obs = observatory[code];
                GeocentricCoord geo_obs = cartesian_to_geocentric(cur_obs.get_cartesian(), *observations[i].get_julian_date());
                observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), geo_obs, earth_position);
                //  std::cout << "IN " <<ind <<" "<< code << " " << map_observatory->at(*observations[i].get_julian_date()).get_x()<< " "<< map_observatory->at(*observations[i].get_julian_date()).get_y() << " " << map_observatory->at(*observations[i].get_julian_date()).get_z() << std::endl;        }
            }
            else 
            {
                GeocentricCoord tmp_position = interpolation_hubble_data(*observations[i].get_julian_date(), hubble_map);
                observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), tmp_position, earth_position);
            }
            //@change len -> length
            delta = n_abs(observations[i].get_barycentric() - observatory_position).length() / (1079252848.8 * 3600.0);
            delta_t -= delta;
        }
        //@change set_julian_date -> set_date
        new_vector.set_date(*observations[i].get_julian_date());
        BarycentricCoord position = interpolation_orbits(delta_t, observations);
        //@change x, y, z -> alpha, beta, gamma; set_position -> set_barycentric_position
        new_vector.set_barycentric_position(position.get_alpha(), position.get_beta(), position.get_gamma());
        new_vector.set_velocity(0, 0, 0);
        result.push_back(new_vector);
    }
    return result;
};

//Нахождение координат по измененной дате (для поправок за световой промежуток)
BarycentricCoord Converter::interpolation_orbits(double date, std::vector<Observation> interpolation_orbits) 
{
    double delta_x;
    double delta_y;
    double delta_z;
    for (int j = 0; j < interpolation_orbits.size(); j++) 
    {
        if (date < (*interpolation_orbits[j].get_julian_date()).get_MJD()) 
        {
            //@change get_x -> alpha
            delta_x = interpolation_orbits[j - 1].get_barycentric().get_alpha() + (interpolation_orbits[j].get_barycentric().get_alpha() - interpolation_orbits[j - 1].get_barycentric().get_alpha()) / ((*interpolation_orbits[j].get_julian_date()).get_MJD() - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD());
            //@change y -> betta
            delta_y = interpolation_orbits[j - 1].get_barycentric().get_beta() + (interpolation_orbits[j].get_barycentric().get_beta() - interpolation_orbits[j - 1].get_barycentric().get_beta()) / ((*interpolation_orbits[j].get_julian_date()).get_MJD() - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD());
            //@change z -> gamma
            delta_z = interpolation_orbits[j - 1].get_barycentric().get_gamma() + (interpolation_orbits[j].get_barycentric().get_gamma() - interpolation_orbits[j - 1].get_barycentric().get_gamma()) / ((*interpolation_orbits[j].get_julian_date()).get_MJD() - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD()) * (date - (*interpolation_orbits[j - 1].get_julian_date()).get_MJD());
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


BarycentricCoord Converter::n_abs(BarycentricCoord frame)
{
    //@TODO Move this method to Barycentric class
    BarycentricCoord result;
    //@change set x, get x -> set alpha, get alpha
    result.set_alpha(std::abs(frame.get_alpha()));
    //@change set y, get y -> set beta, get beta
    result.set_beta(std::abs(frame.get_beta()));
    //@change set z, get z -> set gamma, get gamma
    result.set_gamma(std::abs(frame.get_gamma()));
    return result;
};

//Поправка на гравитационное отклонение света
std::vector<IntegrationVector> Converter::gravitational_deflection(std::map<std::string, Observatory> observatory, std::vector< Observation> observations, std::vector<IntegrationVector> sun_observations, std::vector<InterpolationHubbleFrame> hubble_map, std::vector<IntegrationVector> earth_position) 
{
    double mass_sun = 1.989e30;
    double new_direction[3];
    BarycentricCoord tmp;
    std::vector<IntegrationVector> result;
    std::vector<IntegrationVector>  sun_interpolation = interpolation_to_observation(observations, sun_observations);
    for (int i = 0; i < observations.size(); i++)
    {
        BarycentricCoord observatory_position;
        if (observations[i].get_code() != "250") 
        {
            std::string code = observations[i].get_code();
            Observatory cur_obs = observatory[code];
            GeocentricCoord geo_obs = cartesian_to_geocentric(cur_obs.get_cartesian(), *observations[i].get_julian_date());
            observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), geo_obs, earth_position);
            //  std::cout << "IN " <<ind <<" "<< code << " " << map_observatory->at(*observations[i].get_julian_date()).get_x()<< " "<< map_observatory->at(*observations[i].get_julian_date()).get_y() << " " << map_observatory->at(*observations[i].get_julian_date()).get_z() << std::endl;        }
        }
        else 
        {
            GeocentricCoord tmp_position = interpolation_hubble_data(*observations[i].get_julian_date(), hubble_map);
            observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), tmp_position, earth_position);
        }
        IntegrationVector frame;
        tmp = observations[i].get_barycentric() - observatory_position;
        //@change get_x, y, z -> alpha, beta, gamma
        double direction_observatory_to_asteroid[3] = { tmp.get_alpha(), tmp.get_beta(), tmp.get_gamma() };
        //@change get_position -> get_barycentric_position
        tmp = observations[i].get_barycentric() - sun_interpolation[i].get_barycentric_position();
        //@change get_x, y, z -> alpha, beta, gamma
        double direction_sun_to_asteroid[3] = { tmp.get_alpha(), tmp.get_beta(), tmp.get_gamma() };
        //@change get_position -> get_barycentric_position
        tmp = observatory_position - sun_interpolation[i].get_barycentric_position();
        //@change get_x, y, z -> alpha, beta, gamma
        double direction_sun_to_observatory[3] = { tmp.get_alpha(), tmp.get_beta(), tmp.get_gamma() };
        //@change len() ->  tmp.length();
        double distance_sun_observatory = tmp.length();
        iauLd(mass_sun, direction_observatory_to_asteroid, direction_sun_to_asteroid, direction_sun_to_observatory, distance_sun_observatory, 0, new_direction);
        //@change set_position -> set_barycentric_position
        frame.set_barycentric_position(new_direction[0], new_direction[1], new_direction[2]);
        //@change set_julian_date -> set_date
        frame.set_date(*observations[i].get_julian_date());
        frame.set_velocity(0, 0, 0);
        result.push_back(frame);
    }
    return result;
};

//Аберрация
std::vector<IntegrationVector> Converter::aberration(std::map<std::string, Observatory> observatory, std::vector< Observation> observations, std::vector<IntegrationVector> sun_observations, std::vector<InterpolationHubbleFrame> hubble_map, std::vector<IntegrationVector> earth_position)
{
    std::vector<IntegrationVector> result;
    double new_direction[3];
    double velocity[3] = { 0, 0, 0 };
    BarycentricCoord tmp;
    std::vector<IntegrationVector>  sun_interpolation = interpolation_to_observation(observations, sun_observations);
    for (int i = 0; i < observations.size(); i++)
    {
        BarycentricCoord observatory_position;
        if (observations[i].get_code() != "250")
        {
            std::string code = observations[i].get_code();
            Observatory cur_obs = observatory[code];
            GeocentricCoord geo_obs = cartesian_to_geocentric(cur_obs.get_cartesian(), *observations[i].get_julian_date());
            observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), geo_obs, earth_position);
        }
        else
        {
            GeocentricCoord tmp_position = interpolation_hubble_data(*observations[i].get_julian_date(), hubble_map);
            observatory_position = interpolation_center_of_earth_for_observatory(*observations[i].get_julian_date(), tmp_position, earth_position);
        }
        IntegrationVector frame;
        tmp = observations[i].get_barycentric() - observatory_position;
        //@change  get_x, y, z -> double direction_observatory_to_asteroid[3] = { tmp.get_alpha(), tmp.get_beta(), tmp.get_gamma() };
        double direction_observatory_to_asteroid[3] = { tmp.get_alpha(), tmp.get_beta(), tmp.get_gamma() };
        //@change get_position -> get_barycentric_position
        tmp = observatory_position - sun_interpolation[i].get_barycentric_position();
        //@change double distance_sun_observatory = tmp.len() -> double distance_sun_observatory = tmp.length();
        double distance_sun_observatory = tmp.length();
        iauAb(direction_observatory_to_asteroid, velocity, distance_sun_observatory, 1, new_direction);
        //@change set_position -> set_barycentric_position
        frame.set_barycentric_position(new_direction[0], new_direction[1], new_direction[2]);
        //@change set_julian_date -> set_date
        frame.set_date(*observations[i].get_julian_date());
        frame.set_velocity(0, 0, 0);
        result.push_back(frame);

    }
    return result;
}