#pragma once
#include "../measure/coords/Observatory.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../measure/coords/Observation.h"
#include "../measure/data_structures/HubbleData.h"
#include "../../sofa/src/sofa.h"
#include <map>
#include "../Helpers.h"


class LightCorrector
{
private:
	Helpers help;
public:
	void light_time_correction(std::vector<Observation>* observation, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* sun_info);

	void gravitational_deflection(BarycentricCoord* position_vector, Observatory* observatory, IntegrationVector sun);

	void aberration(BarycentricCoord* corrected_light_position, Observatory* observatory, IntegrationVector sun);
};

