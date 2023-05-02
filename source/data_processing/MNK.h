#pragma once
#include "../measure/data_structures/ModelMeasure.h"
#include "../measure/data_structures/IntegrationVector.h"
#include "Matrix/Matrix.h"
#include "../Helpers.h"
#include <math.h>


class MNK
{
private:
	Helpers help;
public:
	MNK() = default;

	void calculate_dg_dx(ModelMeasure*);
	void calculate_dr_db(ModelMeasure*);

	IntegrationVector Gauss_Newton(IntegrationVector x0, Matrix* A, Matrix* W, Matrix* R);
	Matrix solve_system(Matrix* gradient_f, Matrix* f_b);
};

