#pragma once



class MatrixRow
{
private:
	double* row;
public:
	MatrixRow(double* row) 
	{
		this->row = row;
	};
	double& operator[](int j) 
	{
		return row[j];
	};
	const double& operator[](int j) const 
	{
		return row[j];
	}

	MatrixRow(const MatrixRow& other);
	MatrixRow& operator=(const MatrixRow& other);
	MatrixRow(const MatrixRow&& other);
	MatrixRow& operator=(const MatrixRow&& other);
};

