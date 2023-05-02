#include "MatrixRow.h"

MatrixRow::MatrixRow(double* row)
{
	this->row = row;
}


double& MatrixRow::operator[](int j)
{
	return row[j];
}


const double& MatrixRow::operator[](int j) const
{
	return row[j];
}

