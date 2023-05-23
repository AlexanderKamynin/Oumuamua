#include "LightCorrector.h"



LightCorrector::LightCorrector(Converter* converter, Interpolator* interpolator)
{
	this->converter = converter;
	this->interpolator = interpolator;
}



void LightCorrector::light_correct(std::vector<Observation>* observations, std::vector<IntegrationVector>* model_orbits, std::vector<ModelMeasure>* model_measures, std::vector<IntegrationVector>* sun_info, std::vector<IntegrationVector>* earth_info, std::vector<IntegrationVector>* earth_velocity_info)
{
	std::ofstream delta_output("./output_data/delta.txt");

	for (int i = 0; i < observations->size(); i++)
	{
		BarycentricCoord observatory_position = observations->at(i).get_observatory_position();

		double t = observations->at(i).get_TDB();
		double delta = light_time_correction(t, &observatory_position, model_orbits);
		delta_output << "observatory_code= " << observations->at(i).get_code() << "\tdelta=" << delta << '\n';
		t = t - delta;
		Date time;
		time.set_MJD(t);

		BarycentricCoord object_position = interpolator->find_object_position(time, model_orbits);
		BarycentricCoord sun_position = interpolator->find_object_position(time, sun_info);

		//BarycentricCoord position = object_position - observatory_position;
		//double observer_to_body[3] =
		//{
		//	position.get_x() / position.length(),
		//	position.get_y() / position.length(),
		//	position.get_z() / position.length()
		//};

		//for (int i = 0; i < 3; i++) {
		//	this->corrected_position[i] = observer_to_body[i];
		//}
		

		this->gravitational_deflection(&object_position, &observatory_position, &sun_position);
		Velocity earth_velocity = interpolator->find_earth_velocity(time, earth_velocity_info);
		this->aberration(&observatory_position, &sun_position, &earth_velocity);

		// set corrected position
		ModelMeasure new_state;
		new_state.set_barycentric(this->corrected_position[0], this->corrected_position[1], this->corrected_position[2]);

		this->converter->barycentric_cartesian_to_barycentric_spherical(&new_state);
		model_measures->push_back(new_state);
	}
}


double LightCorrector::light_time_correction(double t, BarycentricCoord* observatory_position, std::vector<IntegrationVector>* model_orbits)
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
	double prevprevdelta = 0;

	Date time;
	time.set_MJD(t - delta);
	BarycentricCoord object_position = interpolator->find_object_position(time, model_orbits);


	double distance = (object_position - *observatory_position).length();
	delta = distance / LIGHT_SPEED;


	while (delta != previous_delta && delta != prevprevdelta)
	{
		prevprevdelta = previous_delta;
		previous_delta = delta;
		time.set_MJD(t - delta);
		object_position = interpolator->find_object_position(time, model_orbits);
		distance = (object_position - *observatory_position).length();
		delta = distance / LIGHT_SPEED;
	}


	return delta;
}


void LightCorrector::gravitational_deflection(BarycentricCoord* body_position, BarycentricCoord* observatory_position, BarycentricCoord* sun_position)
{
	BarycentricCoord position = *body_position - *observatory_position; // observer to body
	double observer_to_body[3] = 
	{
		position.get_x() / position.length(),
		position.get_y() / position.length(),
		position.get_z() / position.length()
	};

	position = *body_position - *sun_position; // sun to body
	double sun_to_body[3] = 
	{ 
		position.get_x() / position.length(), 
		position.get_y() / position.length(), 
		position.get_z() / position.length() 
	};

	position = *observatory_position - *sun_position;
	double sun_to_observer[3] = 
	{ 
		position.get_x() / position.length(),
		position.get_y() / position.length(),
		position.get_z() / position.length() 
	};

	position = *sun_position - *observatory_position;
	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	//double corrected_position[3];
	iauLd(1, observer_to_body, sun_to_body, sun_to_observer, sun_to_observer_length, 0, this->corrected_position);
}


void LightCorrector::aberration(BarycentricCoord* observatory_position, BarycentricCoord* sun_position, Velocity* earth_velocity)
{
	double vx = earth_velocity->get_vx() * 86400; // km/s -> km/day
	double vy = earth_velocity->get_vy() * 86400;
	double vz = earth_velocity->get_vz() * 86400;
	double observer_velocity[3] = { vx, vy, vz};
	double norm_v[3] = { vx / LIGHT_SPEED, vy / LIGHT_SPEED, vz / LIGHT_SPEED };

	double bm1 = std::sqrt(1 - (help.POW_2(norm_v[0]) + help.POW_2(norm_v[1]) + help.POW_2(norm_v[2])));

	BarycentricCoord position = *sun_position - *observatory_position;
	double sun_to_observer_length = position.length() * 6.684589E-09; // km -> au

	double observer_to_body[3];
	for (int i = 0; i < 3; i++)
	{
		observer_to_body[i] = this->corrected_position[i];
	}

	iauAb(observer_to_body, norm_v, sun_to_observer_length, bm1, this->corrected_position);
}
