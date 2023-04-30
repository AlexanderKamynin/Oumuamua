#pragma once
#include "../measure/data_structures/ModelMeasure.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "../Helpers.h"


class MNK
{
private:
	Helpers help;
public:
	MNK() = default;

	void calculate_dg_dx(ModelMeasure*);
	void calculate_dr_db(ModelMeasure*);

	std::vector<IntegrationVector> Gauss_Newton();
};

