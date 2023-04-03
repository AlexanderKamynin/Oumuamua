#pragma once
#include "../measure/coords/Observatory.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/data_structures/HubbleData.h"
#include "../../sofa/src/sofa.h"
#include <map>
#include "Converter.h"
#include "../Helpers.h"
#include <iomanip>


class LightCorrector
{
private:
	Converter* converter;
	Helpers help;
public:
	LightCorrector() = default;
	LightCorrector(Converter*);

	void light_time_correction(std::vector<Observation>* observation, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* model_measure, std::vector<IntegrationVector>* sun_info);

	void gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun);

	void aberration(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun);
};

