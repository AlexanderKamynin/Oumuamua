#pragma once

#include "../measure/coords/CartesianCoord.h"
#include "../measure/coords/GeocentricCoord.h"
#include "../measure/coords/CylindricalCoord.h"
#include "../measure/coords/SphericalCoord.h"
#include "../measure/coords/BarycentricCoord.h"
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"

#include "../measure/time/InterpolationTime.h"

#include "../measure/data_structures/HubbleData.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/data_structures/EarthRotation.h"
#include "../../sofa/src/sofa.h"
#include "../Helpers.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>


// класс для перевода между различными системами
class Converter
{
private:
    Helpers help;
public:
    // convert
    CartesianCoord cylindrical_to_cartesian(CylindricalCoord);
    GeocentricCoord cartesian_to_geocentric(CartesianCoord, Date, EarthRotation);
    void hours_to_degrees_system(Observation*);
    void barycentric_to_spherical(IntegrationVector* vector, std::vector<SphericalCoord>* coords);
    void spherical_to_geocentric(Observation*);
    void UTC_to_TT(Date*);
    void interpolation_time(Date* date_start, std::vector<Observation>* observations, std::vector<InterpolationTime> time);
    void geo_to_bary_for_base_measure(std::vector<Observation>*, std::map<std::string, Observatory>*, std::vector<EarthRotation>*, std::vector<HubbleData>, std::vector<IntegrationVector>);


    // interpolation
    void interpolation_time(std::vector<Observation>*, std::vector<InterpolationTime>);
    BarycentricCoord interpolation_Earth_center(Date date_current, Date date_start, std::vector<IntegrationVector> earth_position);
    BarycentricCoord interpolation_bary_helper(IntegrationVector position_previous, IntegrationVector position_current, Date date);
    std::vector<IntegrationVector> interpolation_model_on_grid(std::vector<Observation> observation_vector, Date* date_start, std::vector<IntegrationVector> interpolation_orbits);
    std::map<std::string, std::vector<IntegrationVector>> interpolation_center_planet(Date*, Date*, double, std::map<std::string, std::vector<IntegrationVector>>);

    // others
    GeocentricCoord find_needed_hubble_data(Date, std::vector<HubbleData>);
};