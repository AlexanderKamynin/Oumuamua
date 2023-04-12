#include "LightCorrector.h"


LightCorrector::LightCorrector(Converter* converter)
{
	this->converter = converter;
}

void LightCorrector::light_correct(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* model_measure, std::vector<IntegrationVector>* sun_info)
{
	for (int i = 0; i < observations->size(); i++)
	{
		Observatory current_observatory = observatory->at(observations->at(i).get_code());
		double t = observations->at(i).get_date()->get_MJD();

		t = t - light_time_correction(t, &current_observatory, model_measure);
		Date time;
		time.set_MJD(t);
		BarycentricCoord object_position = this->find_object_position(time, model_measure);
		BarycentricCoord sun_position = this->find_object_position(time, sun_info);

		BarycentricCoord observatory_position = current_observatory.get_barycentric();
		// gravitational deflection
		this->gravitational_deflection(&object_position, &observatory_position, &sun_position);
		// abberation
		this->aberration(&object_position, &observatory_position, &sun_position);

		// set corrected position
		observations->at(i).set_barycentric(object_position);
	}
}


double LightCorrector::light_time_correction(double t, Observatory* observatory, std::vector<IntegrationVector>* model_measure)
{
	/*
		| object (t - delta) - observer(t) | = c * delta
		delta = | object (t - delta) - observer(t)| / c

		delta0 = 0
		delta1 = | object (t - delta0) - observer(t)| / c
		delta2 = | object (t - delta1) - observer(t)| / c
		...

		while deltai - deltai-1 < accuracy
	*/

	double delta = 0.0;
	double previous_delta = 0;

	Date time;
	time.set_MJD(t - delta);
	BarycentricCoord object_position = this->find_object_position(time, model_measure);
	
	
	double distance = (object_position - observatory->get_barycentric()).length();
	delta = distance / LIGHT_SPEED;

	while (previous_delta == 0 or std::fabs(delta - previous_delta) > 1e15)
	{
		previous_delta = delta;
		time.set_MJD(t - delta);
		object_position = this->find_object_position(time, model_measure);
		distance = (object_position - observatory->get_barycentric()).length();
		delta = distance / LIGHT_SPEED;
	}

	return delta;
}


BarycentricCoord LightCorrector::find_object_position(Date time, std::vector<IntegrationVector>* model_measure)
{
	BarycentricCoord object_position;
	int idx = int(((time.get_MJD()) - model_measure->at(0).get_date().get_MJD()) / STEP); // search for needed time
	if (idx == 0)
	{
		object_position = model_measure->at(0).get_barycentric_position();
	}
	else
	{
		object_position = converter->interpolation_helper(model_measure->at(idx), model_measure->at(idx - 1), time);
	}

	return object_position;
}


void LightCorrector::gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun_position)
{
	double sun_mass = 1; // mass in solar masses
	double observer_to_body[3] = 
	{
		body_position->get_alpha() / body_position->length(),
		body_position->get_beta() / body_position->length(),
		body_position->get_gamma() / body_position->length()
	};
	double corrected_position[3];

	BarycentricCoord position = *body_position - *sun_position; // sun to body
	double sun_to_body[3] = 
	{ 
		position.get_alpha() / position.length(), 
		position.get_beta() / position.length(), 
		position.get_gamma() / position.length() 
	};

	position = *observatory_position - *sun_position;
	double sun_to_observer[3] = 
	{ 
		position.get_alpha() / position.length(),
		position.get_beta() / position.length(),
		position.get_gamma() / position.length() 
	};

	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	iauLd(sun_mass, observer_to_body, sun_to_body, sun_to_observer, sun_to_observer_length, 0, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * vector_length);
	body_position->set_beta(corrected_position[1] * vector_length);
	body_position->set_gamma(corrected_position[2] * vector_length);
}


void LightCorrector::aberration(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun_position)
{
	double corrected_position[3];
	double observer_to_body[3] =
	{ 
		body_position->get_alpha() / body_position->length(),
		body_position->get_beta() / body_position->length(),
		body_position->get_gamma() / body_position->length()
	};
	double observer_velocity[3] = { 0, 0, 0};

	BarycentricCoord position = *observatory_position - *sun_position;
	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	iauAb(observer_to_body, observer_velocity, sun_to_observer_length, 1, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * body_position->length());
	body_position->set_beta(corrected_position[1] * body_position->length());
	body_position->set_gamma(corrected_position[2] * body_position->length());
}

