#include "LightCorrector.h"


LightCorrector::LightCorrector(Converter* converter)
{
	this->converter = converter;
}

void LightCorrector::light_correct(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* model_measure, std::vector<IntegrationVector>* sun_info, std::vector<IntegrationVector>* earth_velocity_info)
{
	std::ofstream delta_output("./output_data/delta.txt");

	for (int i = 0; i < observations->size(); i++)
	{
		Observatory current_observatory = observatory->at(observations->at(i).get_code());
		double t = observations->at(i).get_date()->get_MJD();

		double delta = light_time_correction(t, &current_observatory, model_measure);
		delta_output << delta << '\n';

		t = t - delta;
		Date time;
		time.set_MJD(t);
		BarycentricCoord object_position = this->find_object_position(time, model_measure);
		BarycentricCoord sun_position = this->find_object_position(time, sun_info);

		BarycentricCoord observatory_position = current_observatory.get_barycentric();
		// gravitational deflection
		this->gravitational_deflection(&object_position, &observatory_position, &sun_position);

		Velocity earth_velocity = this->find_earth_velocity(time, earth_velocity_info);
		//this->aberration(&object_position, &observatory_position, &sun_position, &earth_velocity);

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
	double previous_delta = 0.0;

	Date time;
	time.set_MJD(t - delta);
	BarycentricCoord object_position = this->find_object_position(time, model_measure);
	
	
	double distance = (object_position - observatory->get_barycentric()).length();
	delta = distance / LIGHT_SPEED;

	while (previous_delta == 0 or std::fabs(delta - previous_delta) / delta > 1e-15)
	{
		previous_delta = delta;
		time.set_MJD(t - delta);
		object_position = this->find_object_position(time, model_measure);
		distance = (object_position - observatory->get_barycentric()).length();
		delta = distance / LIGHT_SPEED;
	}

	return delta;
}


void LightCorrector::gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun_position)
{
	BarycentricCoord position = *body_position;
	double observer_to_body[3] = 
	{
		position.get_alpha() / position.length(),
		position.get_beta() / position.length(),
		position.get_gamma() / position.length()
	};

	position = *body_position - *sun_position; // sun to body
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

	position = *sun_position - *observatory_position;
	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	double corrected_position[3];
	iauLd(1, observer_to_body, sun_to_body, sun_to_observer, sun_to_observer_length, 0, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * vector_length);
	body_position->set_beta(corrected_position[1] * vector_length);
	body_position->set_gamma(corrected_position[2] * vector_length);
}


void LightCorrector::aberration(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun_position, Velocity* earth_velocity)
{
	double corrected_position[3];
	double observer_to_body[3] =
	{ 
		body_position->get_alpha() / body_position->length(),
		body_position->get_beta() / body_position->length(),
		body_position->get_gamma() / body_position->length()
	};
	double vx = earth_velocity->get_vx() * 86400; // km/s -> km/day
	double vy = earth_velocity->get_vy() * 86400;
	double vz = earth_velocity->get_vz() * 86400;
	double observer_velocity[3] = { vx, vy, vz};
	double norm_v[3] = { vx / LIGHT_SPEED, vy / LIGHT_SPEED, vz / LIGHT_SPEED };

	double bm1 = std::sqrt(1 - (help.POW_2(norm_v[0]) + help.POW_2(norm_v[1]) + help.POW_2(norm_v[2])));

	BarycentricCoord position = *observatory_position - *sun_position;
	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	iauAb(observer_to_body, observer_velocity, sun_to_observer_length, bm1, corrected_position);

	double vector_length = body_position->length();
	body_position->set_alpha(corrected_position[0] * vector_length);
	body_position->set_beta(corrected_position[1] * vector_length);
	body_position->set_gamma(corrected_position[2] * vector_length);
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


Velocity LightCorrector::find_earth_velocity(Date time, std::vector<IntegrationVector>* earth_velocity_info)
{
	Velocity earth_velocity;
	int last = 0;
	std::vector<IntegrationVector> result;
	double step = earth_velocity_info->at(1).get_date().get_MJD() - earth_velocity_info->at(0).get_date().get_MJD();
	int idx = int(((time.get_MJD()) - earth_velocity_info->at(0).get_date().get_MJD()) / step); // search for needed time
	if (idx == 0)
	{
		earth_velocity = earth_velocity_info->at(0).get_velocity();
	}
	else
	{
		Velocity current_velocity = earth_velocity_info->at(idx).get_velocity();
		Velocity previous_velocity = earth_velocity_info->at(idx - 1).get_velocity();
		double t_current = earth_velocity_info->at(idx).get_date().get_MJD();
		double t_previous = earth_velocity_info->at(idx - 1).get_date().get_MJD();
		double t_interpolate = time.get_MJD();

		earth_velocity = previous_velocity + (current_velocity - previous_velocity) / (t_current - t_previous) * (t_interpolate - t_previous) ;
	}
	return earth_velocity;
}
