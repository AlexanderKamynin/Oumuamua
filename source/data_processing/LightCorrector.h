#pragma once
#include "../measure/coords/Observatory.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/data_structures/HubbleData.h"
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
	Helpers help;
public:
	LightCorrector() = default;
	LightCorrector(Converter*);

	void light_correct(std::vector<Observation>* observation, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* model_measure, std::vector<IntegrationVector>* sun_info, std::vector<IntegrationVector>* earth_velocity_info);

	double light_time_correction(double t, Observatory* observatory, std::vector<IntegrationVector>* model_measure);

	void gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun);

	void aberration(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun, Velocity* earth_velocity);

	BarycentricCoord find_object_position(Date time, std::vector<IntegrationVector>* model_measure);

	Velocity find_earth_velocity(Date time, std::vector<IntegrationVector>* earth_velocity_info);
};

