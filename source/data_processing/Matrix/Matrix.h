#pragma once
#include <iostream>
#include <math.h>
#include <vector>
#include "MatrixRow.h"


class Matrix
{
private:
	int n = 0;
	int m = 0;
	double** matrix = nullptr;
public:
	Matrix(int n, int m);
	Matrix(int n, int m, std::vector<std::vector<double>>);

	int rows() const;
	int columns() const;
	bool is_empty();
	Matrix transpose();
	void make_identity();
	Matrix Cholesky_decomposition(Matrix);

	Matrix(const Matrix& other);
	Matrix& operator=(const Matrix& other);
	Matrix(const Matrix&& other);
	Matrix& operator=(const Matrix&& other);


	MatrixRow operator[](int i) const;

	friend std::ostream& operator<<(std::ostream& outstream, const Matrix mtr);
	friend Matrix operator+(Matrix const A, Matrix const B);
	friend Matrix operator-(Matrix const A, Matrix const B);
	friend Matrix operator*(Matrix const A, Matrix const B);
	friend Matrix operator*(double const factor, Matrix const A);
	friend Matrix operator*(Matrix const A, double const factor);
	~Matrix();
};
