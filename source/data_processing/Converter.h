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
    void celestial_to_spherical(Observation*);
    void barycentric_to_spherical(IntegrationVector*);
    void spherical_to_geocentric(Observation*);
    void geocentric_to_barycentric(std::vector<Observation>*, std::map<std::string, Observatory>*, std::vector<EarthRotation>*, std::vector<HubbleData>, std::vector<IntegrationVector>);
    void barycentric_to_spherical_for_observations(Observation*);
    void UTC_to_TT(Date*);

    // interpolation
    void interpolation_time(std::vector<Observation>*, std::vector<InterpolationTime>);
    std::vector<IntegrationVector> interpolation_to_observation(std::vector<Observation>, std::vector<IntegrationVector>);
    BarycentricCoord interpolation_orbits(double date, std::vector<Observation> interpolation_orbits);
    BarycentricCoord interpolation_Earth_center(Date, GeocentricCoord, std::vector<IntegrationVector>);
    std::map<std::string, std::vector<IntegrationVector>> interpolation_center_planet(double h, Date* date_start, Date* date_finish, std::map<std::string, std::vector<IntegrationVector>> interpolation_planet);

    //@TODO added the method for correct the data (light time correction, Gravitational deflection of light, Light aberration)
    
    // others
    GeocentricCoord find_needed_hubble_data(Date, std::vector<HubbleData>);
};

