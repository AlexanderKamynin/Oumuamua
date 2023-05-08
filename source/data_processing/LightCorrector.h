#pragma once
#include "../measure/coords/Observatory.h"
#include "../data_processing/Interpolator.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/data_structures/HubbleData.h"
#include "../measure/data_structures/ModelMeasure.h"
#include "../../sofa/src/sofa.h"
#include <map>
#include "Converter.h"
#include "../Helpers.h"
#include "../measure/movement/Velocity.h"
#include <iomanip>
#include <iostream>



class LightCorrector
{
private:
	Converter* converter;
	Interpolator* interpolator;
	Helpers help;
public:
	LightCorrector() = default;
	LightCorrector(Converter*, Interpolator*);

	void light_correct(std::vector<Observation>* observation, std::vector<IntegrationVector>* model_orbit, std::vector<ModelMeasure>* model_measures, std::vector<IntegrationVector>* sun_info, std::vector<IntegrationVector>* earth_velocity_info);

	double light_time_correction(double t, BarycentricCoord* observatory_position, std::vector<IntegrationVector>* model_measure);

	void gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun);

	void aberration(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun, Velocity* earth_velocity);

};

