#pragma once
#define EARTH_RADIUS 6378.140

#include "./measure/CartesianCoord.h"
#include "./measure/GeocentricCoord.h"
#include "./measure/CylindricalCoord.h"
#include "./measure/Observation.h"
#include "./measure/Observatory.h"
#include "./measure/InterpolationTimeFrame.h"
#include "./measure/InterpolationHubbleFrame.h"
#include "./measure/IntegrationVector.h"
#include "../sofa/src/sofa.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>


// ����� ��� �������� ����� ���������� ���������
class Converter
{
public:

    CartesianCoord cylindrical_to_cartesian(CylindricalCoord measure);
    void julian_date_to_tt(Date*);
    void interpolation_date_to_tt_tdb(std::vector<Observation>* observations, std::vector<InterpolationTimeFrame> interpolation_time);
    void interpolation_to_tdb(Date*, std::vector<InterpolationTimeFrame>);


    GeocentricCoord interpolation_hubble_data(Date date, std::vector<InterpolationHubbleFrame> interpolation_data);
    GeocentricCoord cartesian_to_geocentric(CartesianCoord, Date);
    BarycentricCoord interpolation_center_of_earth_for_observatory(Date date, GeocentricCoord frame, std::vector<IntegrationVector> interpolation_earth);
    std::map<std::string, std::vector<IntegrationVector>> interpolation_center_planet(double h, Date* date_start, Date* date_finish, std::map<std::string, std::vector<IntegrationVector>> interpolation_planet);
    void geocentric_to_barycentric(std::vector<Observation>*, std::map<std::string, Observatory>*, std::vector<InterpolationHubbleFrame>, std::vector<IntegrationVector>);


    void transpose(double mtr[3][3]);

    std::vector<IntegrationVector> light_time_correction(std::map<std::string, Observatory>, std::vector< Observation>, std::vector<InterpolationHubbleFrame>, std::vector<IntegrationVector> earth_position);
    std::vector<IntegrationVector> interpolation_to_observation(std::vector<Observation> vector, std::vector<IntegrationVector> interpolation_orbits);
    BarycentricCoord interpolation_orbits(double date, std::vector<Observation> interpolation_orbits);
    BarycentricCoord n_abs(BarycentricCoord);
    std::vector<IntegrationVector> gravitational_deflection(std::map<std::string, Observatory>, std::vector< Observation>, std::vector<IntegrationVector>, std::vector<InterpolationHubbleFrame>, std::vector<IntegrationVector> earth_position);
    std::vector<IntegrationVector> aberration(std::map<std::string, Observatory>, std::vector< Observation>, std::vector<IntegrationVector>, std::vector<InterpolationHubbleFrame>, std::vector<IntegrationVector> earth_position);

    void celestial_to_spherical(Observation*);
    void barycentric_to_spherical(IntegrationVector*);
    void spherical_to_geocentric(Observation*);

    void barycentric_to_spherical_for_observations(Observation*);
};

