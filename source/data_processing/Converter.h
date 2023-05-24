#pragma once
#include "../measure/coords/CartesianCoord.h"
#include "../measure/coords/GeocentricCoord.h"
#include "../measure/coords/CylindricalCoord.h"
#include "../measure/coords/SphericalCoord.h"
#include "../measure/coords/BarycentricCoord.h"
#include "../measure/coords/Observation.h"
#include "../measure/coords/Observatory.h"
#include "../measure/data_structures/ModelMeasure.h"
#include "../measure/time/InterpolationTime.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/data_structures/EarthRotation.h"
#include "./Interpolator.h"
#include "../../sofa/src/sofa.h"
#include "../Helpers.h"

#include <vector>
#include <string>
#include <iostream>
#include <map>



// Class for converting between different systems
class Converter
{
private:
    Helpers help;
    Interpolator* interpolator;
    std::vector<TT_TDB_obj>* tdb_grid;
public:
    Converter() = default;
    Converter(Interpolator*);

    void set_tdb_grid(std::vector<TT_TDB_obj>* tdb_grid);

    // convert
    CartesianCoord cylindrical_to_cartesian(CylindricalCoord);
    GeocentricCoord terrestial_to_geocentric_celestial(CartesianCoord, Date, Date, EarthRotation);
    void spherical_hours_to_spherical_radians(Observation*);
    void barycentric_cartesian_to_geocentric_cartesian(ModelMeasure* model_measure, std::vector<IntegrationVector>* earth);
    void barycentric_cartesian_to_geocentric_cartesian(IntegrationVector* model_measure, std::vector<IntegrationVector>* earth);
    void geocentric_cartesian_to_geocentric_spherical(ModelMeasure* model_measure);
    void barycentric_cartesian_to_barycentric_spherical(ModelMeasure* model_measure);
    void barycentric_spherical_to_geocentric_cartesian(Observation*);
    void cartesian_geocentric_to_cartesian_barycentric(std::vector<Observation>*, std::map<std::string, Observatory>*, std::vector<EarthRotation>*, std::vector<IntegrationVector>*, std::vector<IntegrationVector>*);
    void UTC_to_TT(Date*, Observation* observation);
};