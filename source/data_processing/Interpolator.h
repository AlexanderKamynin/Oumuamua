#pragma once
#include "../Helpers.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"
#include "../measure/time/InterpolationTime.h"
#include "../measure/time/Date.h"
#include "../measure/movement/Velocity.h"
#include "../data_processing/Matrix/Matrix.h"
#include "../measure/time/InterpolationTime.h"

#include <map>
#include <iostream>
#include <vector>
#include <string>


class Interpolator
{
private:

public:
	Interpolator() = default;

    double interpolation_time(double time, std::vector<TT_TDB_obj>* tdb_grid);
    BarycentricCoord interpolation_helper(Date date, IntegrationVector position_previous, IntegrationVector position_current);
    std::vector<IntegrationVector> interpolation_model_on_grid(std::vector<Observation> observation_vector, Date* date_start, std::vector<IntegrationVector> interpolation_orbits);
    std::map<std::string, std::vector<IntegrationVector>> interpolation_center_planet(Date*, Date*, double, std::map<std::string, std::vector<IntegrationVector>>);

    BarycentricCoord find_object_position(Date time, std::vector<IntegrationVector>* model_measure);
    Velocity find_earth_velocity(Date time, std::vector<IntegrationVector>* earth_velocity_info);
    Velocity find_orbit_velocity(Date time, std::vector<IntegrationVector>* orbit_velocity_info);

    Matrix interpolate_dx_db(Date time, std::vector<IntegrationVector>* model);
};

