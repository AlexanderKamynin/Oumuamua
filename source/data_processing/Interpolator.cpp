#include "Interpolator.h"


/*
    Interpolation of time on a uniform grid for observations
*/
void Interpolator::interpolation_time(Date* date_start, std::vector<Observation>* observations, std::vector<InterpolationTime> time)
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
        double TDB = observations->at(i).get_date()->get_TT() - (interpolation_time_term / 86400000);
        observations->at(i).get_date()->set_TDB(TDB);
    }
}



/*
    Interpolation numerical integraion result (model) on the time grid
*/
std::vector<IntegrationVector> Interpolator::interpolation_model_on_grid(std::vector<Observation> observation_vector, Date* date_start, std::vector<IntegrationVector> interpolation_orbits)
{
    int last = 0;
    std::vector<IntegrationVector> result;
    for (int i = 0; i < observation_vector.size(); i++)
    {
        IntegrationVector interpolated_vector;
        int j = int((observation_vector[i].get_date()->get_MJD() - date_start->get_MJD()) / STEP) + 2;
        BarycentricCoord interpolated_position = interpolation_helper(*observation_vector[i].get_date(), interpolation_orbits[j], interpolation_orbits[j - 1]);
        Date new_date = *observation_vector[i].get_date();
        interpolated_vector.set_date(new_date);
        interpolated_vector.set_barycentric(interpolated_position.get_x(), interpolated_position.get_y(), interpolated_position.get_z());
        interpolated_vector.set_velocity(interpolation_orbits[j].get_velocity().get_vx(), interpolation_orbits[j].get_velocity().get_vy(), interpolation_orbits[j].get_velocity().get_vz());
        result.push_back(interpolated_vector);
    }
    return result;
}



/*
    Interpolation all center planets coordinates
*/
std::map<std::string, std::vector<IntegrationVector>> Interpolator::interpolation_center_planet(Date* date_start, Date* date_end, double step, std::map<std::string, std::vector<IntegrationVector>> planets_position)
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
                    BarycentricCoord interpolated_position_1 = interpolation_helper(current_date, interpolation_planet.second[j], interpolation_planet.second[j - 1]);
                    IntegrationVector interpolated_position;
                    interpolated_position.set_date(current_date);
                    interpolated_position.set_barycentric(interpolated_position_1.get_x(), interpolated_position_1.get_y(), interpolated_position_1.get_z());
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
BarycentricCoord Interpolator::interpolation_Earth_center(Date date_current, Date date_start, std::vector<IntegrationVector> earth_position)
{
    int i = int((date_current.get_MJD() - date_start.get_MJD()) / STEP) + 1;
    BarycentricCoord interpolated_position = interpolation_helper(date_current, earth_position[i], earth_position[i - 1]);
    return interpolated_position;
}



BarycentricCoord Interpolator::interpolation_helper(Date date, IntegrationVector position_current, IntegrationVector position_previous)
{
    BarycentricCoord f_current = position_current.get_barycentric();
    BarycentricCoord f_previous = position_previous.get_barycentric();
    double t_current = position_current.get_date().get_MJD();
    double t_previous = position_previous.get_date().get_MJD();
    double t_interpolate = date.get_MJD();

    BarycentricCoord interpolated_position = f_previous + (f_current - f_previous) / (t_current - t_previous) * (t_interpolate - t_previous);

    interpolated_position.set_x(interpolated_position.get_x());
    interpolated_position.set_y(interpolated_position.get_y());
    interpolated_position.set_z(interpolated_position.get_z());
    return interpolated_position;
}


BarycentricCoord Interpolator::find_object_position(Date time, std::vector<IntegrationVector>* model_measure)
{
    BarycentricCoord object_position;
    int idx = int(((time.get_MJD()) - model_measure->at(0).get_date().get_MJD()) / STEP); // search for needed time
    if (idx == 0)
    {
        object_position = model_measure->at(0).get_barycentric();
    }
    else
    {
        object_position = this->interpolation_helper(time, model_measure->at(idx), model_measure->at(idx - 1));
    }

    return object_position;
}


Velocity Interpolator::find_earth_velocity(Date time, std::vector<IntegrationVector>* earth_velocity_info)
{
    Velocity earth_velocity;
    std::vector<IntegrationVector> result;
    double step = earth_velocity_info->at(1).get_date().get_MJD() - earth_velocity_info->at(0).get_date().get_MJD();
    int idx = int(((time.get_MJD()) - earth_velocity_info->at(0).get_date().get_MJD()) / step); // search for needed time
    if (idx == 0)
    {
        earth_velocity = earth_velocity_info->at(0).get_velocity();
    }
    else
    {
        Velocity current_velocity = earth_velocity_info->at(idx).get_velocity();
        Velocity previous_velocity = earth_velocity_info->at(idx - 1).get_velocity();
        double t_current = earth_velocity_info->at(idx).get_date().get_MJD();
        double t_previous = earth_velocity_info->at(idx - 1).get_date().get_MJD();
        double t_interpolate = time.get_MJD();

        earth_velocity = previous_velocity + (current_velocity - previous_velocity) / (t_current - t_previous) * (t_interpolate - t_previous);
    }
    return earth_velocity;
}


Velocity Interpolator::find_orbit_velocity(Date time, std::vector<IntegrationVector>* orbit_velocity_info)
{
    Velocity earth_velocity;
    std::vector<IntegrationVector> result;
    double step = STEP; // set step from integration
    int idx = int(((time.get_MJD()) - orbit_velocity_info->at(0).get_date().get_MJD()) / step); // search for needed time
    if (idx == 0)
    {
        earth_velocity = orbit_velocity_info->at(0).get_velocity();
    }
    else
    {
        Velocity current_velocity = orbit_velocity_info->at(idx).get_velocity();
        Velocity previous_velocity = orbit_velocity_info->at(idx - 1).get_velocity();
        double t_current = orbit_velocity_info->at(idx).get_date().get_MJD();
        double t_previous = orbit_velocity_info->at(idx - 1).get_date().get_MJD();
        double t_interpolate = time.get_MJD();

        earth_velocity = previous_velocity + (current_velocity - previous_velocity) / (t_current - t_previous) * (t_interpolate - t_previous);
    }
    return earth_velocity;
}

Matrix Interpolator::interpolate_dx_db(Date time, std::vector<IntegrationVector>* model)
{
    Matrix interpolated_dx_db = Matrix(6, 6);
    double step = STEP;
    int idx = int(((time.get_MJD()) - model->at(0).get_date().get_MJD()) / step);
    if (idx == 0)
    {
        interpolated_dx_db = *model->at(0).get_dx_db();
    }
    else
    {
        double t_current = model->at(idx).get_date().get_MJD();
        double t_previous = model->at(idx - 1).get_date().get_MJD();
        double t_interpolate = time.get_MJD();

        interpolated_dx_db = (*model->at(idx-1).get_dx_db()) + ((*model->at(idx).get_dx_db()) - (*model->at(idx - 1).get_dx_db())) * (1 / (t_current - t_previous)) * (t_interpolate - t_previous);
    }

    return interpolated_dx_db;
}
