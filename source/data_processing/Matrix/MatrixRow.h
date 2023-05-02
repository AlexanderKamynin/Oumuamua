#pragma once



class MatrixRow
{
private:
	double* row;
public:
	MatrixRow(double* row);


	double& operator[](int j);
	const double& operator[](int j) const;
	MatrixRow(const MatrixRow& other);
	MatrixRow& operator=(const MatrixRow& other);
	MatrixRow(const MatrixRow&& other);
	MatrixRow& operator=(const MatrixRow&& other);
};

