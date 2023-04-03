#include "LightCorrector.h"


void LightCorrector::light_time_correction(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* sun_info)
{
	/*
		| object (t - delta) - observer(t) | = c * delta

		t - delta = t_obs
		delta = t - t_obs
		delta = | object (t_obs) - observer(t)| / c

		object(t) = delta * V_object, where V_object = {vx, vy, vz}
	*/
	for (int i = 0; i < observations->size(); i++)
	{
		BarycentricCoord observatory_position;
        Observatory current_observatory = observatory->at(observations->at(i).get_code());

		double delta = (observations->at(i).get_barycentric() - current_observatory.get_barycentric()).length() / LIGHT_SPEED;
		
		BarycentricCoord corrected_position = observations->at(i).get_barycentric();

		this->gravitational_deflection(&corrected_position, &current_observatory, sun_info->at(0));
		this->aberration(&corrected_position, &current_observatory, sun_info->at(0));

		observations->at(i).set_barycentric(corrected_position);
	}
}


void LightCorrector::gravitational_deflection(BarycentricCoord* body_position, Observatory* observatory, IntegrationVector sun)
{
	double sun_mass = SUN_MASS;
	double observer_to_body[3] = { body_position->get_alpha(), body_position->get_beta(), body_position->get_gamma() };
	double corrected_position[3];

	BarycentricCoord position = *body_position - sun.get_barycentric_position(); // sun to body
	double sun_to_body[3] = { position.get_alpha() / position.length(), position.get_beta() / position.length(), position.get_gamma() / position.length() };

	position = observatory->get_barycentric() - sun.get_barycentric_position();
	double sun_to_observer[3] = { position.get_alpha() / position.length(), position.get_beta() / position.length(), position.get_gamma() / position.length() };

	double sun_to_observer_length = position.length();

	iauLd(sun_mass, observer_to_body, sun_to_body, sun_to_observer, sun_to_observer_length, 0, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * vector_length);
	body_position->set_beta(corrected_position[1] * vector_length);
	body_position->set_gamma(corrected_position[2] * vector_length);
}


void LightCorrector::aberration(BarycentricCoord* body_position, Observatory* observatory, IntegrationVector sun)
{
	double corrected_position[3];
	double observer_to_body[3] = { body_position->get_alpha(), body_position->get_beta(), body_position->get_gamma() };
	double observer_velocity[3] = { 0, 0, 0 };

	BarycentricCoord position = observatory->get_barycentric() - sun.get_barycentric_position();
	double sun_to_observer_length = position.length();

	iauAb(observer_to_body, observer_velocity, sun_to_observer_length, 1, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * vector_length);
	body_position->set_beta(corrected_position[1] * vector_length);
	body_position->set_gamma(corrected_position[2] * vector_length);
}

