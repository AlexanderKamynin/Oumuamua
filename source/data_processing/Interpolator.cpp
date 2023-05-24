#include "Interpolator.h"



double Interpolator::interpolation_time(double time, std::vector<TT_TDB_obj>* tdb_grid)
{
    double step = tdb_grid->at(1).get_date().get_MJD() - tdb_grid->at(0).get_date().get_MJD();
    int idx = int((time - tdb_grid->at(0).get_date().get_MJD()) / step);
    if (idx == 0) {
        return tdb_grid->at(0).get_TT_TDB();
    }
    else
    {
        double f_current = tdb_grid->at(idx).get_TT_TDB();
        double f_previous = tdb_grid->at(idx - 1).get_TT_TDB();
        double t_current = tdb_grid->at(idx).get_date().get_MJD();
        double t_previous = tdb_grid->at(idx - 1).get_date().get_MJD();

        return f_previous + (f_current - f_previous) / (t_current - t_previous) * (time - t_previous);
    }
};


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


BarycentricCoord Interpolator::find_object_position(Date time, std::vector<IntegrationVector>* object)
{
    BarycentricCoord object_position;
    double step = object->at(1).get_date().get_MJD() - object->at(0).get_date().get_MJD();
    int idx = int(((time.get_MJD()) - object->at(0).get_date().get_MJD()) / step); // search for needed time
    if (idx <= 0)
    {
        object_position = object->at(0).get_barycentric();
    }
    else
    {
        object_position = this->interpolation_helper(time, object->at(idx), object->at(idx - 1));
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
