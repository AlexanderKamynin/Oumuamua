#pragma once
#include "../Helpers.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"
#include "../measure/time/InterpolationTime.h"
#include "../measure/time/Date.h"

#include <map>
#include <iostream>
#include <vector>
#include <string>


class Interpolator
{
private:

public:
	Interpolator() = default;

    // interpolation
    void interpolation_time(Date* date_start, std::vector<Observation>* observations, std::vector<InterpolationTime> time);
    BarycentricCoord interpolation_Earth_center(Date date_current, Date date_start, std::vector<IntegrationVector> earth_position);
    BarycentricCoord interpolation_helper(IntegrationVector position_previous, IntegrationVector position_current, Date date);
    std::vector<IntegrationVector> interpolation_model_on_grid(std::vector<Observation> observation_vector, Date* date_start, std::vector<IntegrationVector> interpolation_orbits);
    std::map<std::string, std::vector<IntegrationVector>> interpolation_center_planet(Date*, Date*, double, std::map<std::string, std::vector<IntegrationVector>>);
    BarycentricCoord find_object_position(Date time, std::vector<IntegrationVector>* model_measure);
    Velocity find_earth_velocity(Date time, std::vector<IntegrationVector>* earth_velocity_info);
};

