#include "LightCorrector.h"


LightCorrector::LightCorrector(Converter* converter)
{
	this->converter = converter;
}


void LightCorrector::light_time_correction(std::vector<Observation>* observations, std::map<std::string, Observatory>* observatory, std::vector<IntegrationVector>* model_measure, std::vector<IntegrationVector>* sun_info)
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
	double accuracy = 1e-6;

	for (int i = 0; i < model_measure->size(); i++)
	{
		double delta_current = 0;
		double delta_prev = 0;
		double t = model_measure->at(i).get_date().get_MJD();
		BarycentricCoord body_position = model_measure->at(i).get_barycentric_position();
		BarycentricCoord observatory_position = observatory->at(observations->at(i).get_code()).get_barycentric();

		while (delta_current == 0 or std::abs(delta_current - delta_prev) >= accuracy)
		{
			delta_prev = delta_current;
			delta_current = (body_position - observatory_position).length() / LIGHT_SPEED;

			// get  object(t - deltai)
			Date model_time;
			model_time.set_MJD(t - delta_current);
			int idx = 1;
			for (int j = 1; j < model_measure->size(); j++)
			{
				if (model_measure->at(j).get_date().get_MJD() > model_time.get_MJD())
				{
					idx = j;
					break;
				}
			}
			BarycentricCoord body_position = converter->interpolation_helper(model_measure->at(idx), model_measure->at(idx-1), model_time);
		}

		Date date_to_interpolate;
		date_to_interpolate.set_MJD(t - delta_current);
		int idx = 1;
		for (int j = 1; j < sun_info->size(); j++)
		{
			if (date_to_interpolate.get_MJD() < model_measure->at(j).get_date().get_MJD())
			{
				idx = j;
				break;
			}
		}
		BarycentricCoord sun_position = converter->interpolation_helper(sun_info->at(idx), sun_info->at(idx - 1), date_to_interpolate);

		// gravitational deflection
		this->gravitational_deflection(&body_position, &observatory_position, &sun_position);
		// abberation
		this->aberration(&body_position, &observatory_position, &sun_position);

		// set corrected position
		model_measure->at(i).set_barycentric_position(body_position.get_alpha(), body_position.get_beta(), body_position.get_gamma());
	}
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

