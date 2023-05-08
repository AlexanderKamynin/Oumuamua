#include "Matrix.h"

Matrix::Matrix(int n, int m) 
{
    this->matrix = new double* [n];
    this->rows_count = n;
    this->columns_count = m;
    for (int i = 0; i < n; i++)
    {
        this->matrix[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            this->matrix[i][j] = 0;
        }
    }
}

int Matrix::rows() const
{
    return rows_count;
}

int Matrix::columns() const
{
    return columns_count;
}

void Matrix::make_identity()
{
    int min_dimension = std::min(rows_count, columns_count);
    for (int i = 0; i < min_dimension; i++)
    {
        for (int j = 0; j < min_dimension; j++)
        {
            if (i == j)
            {
                this->matrix[i][j] = 1;
            }
            else 
            {
                this->matrix[i][j] = 0;
            }
        }
    }
}

Matrix Matrix::Cholesky_decomposition(Matrix A)
{
    Matrix L(A.rows(), A.columns());
    if (L.rows_count != L.columns_count)
    {
        std::cout << "Error :: Not square matrix in Cholesky_decomposition!" << std::endl;
        return L;
    }
    int size = A.rows();
    for (int i = 0; i < size; i++) 
    {
        for (int j = 0; j <= i; j++) 
        {
            double sum = 0;
            for (int k = 0; k < j; k++)
                sum += L[i][k] * L[j][k];

            if (i == j)
                L[i][j] = std::sqrt(A[i][i] - sum);
            else
                L[i][j] = (1.0 / L[j][j] * (A[i][j] - sum));
        }
    }
    return L;
}

Matrix Matrix::transpose()
{
    Matrix transposed_matrix(columns_count, rows_count);
    for (int i = 0; i < rows_count; i++)
    {
        for (int j = 0; j < columns_count; j++)
        {
            transposed_matrix[j][i] = this->matrix[i][j];
        }
    }
    return transposed_matrix;
}


Matrix::Matrix(const Matrix& other)
{
    if ((this->rows_count != other.rows_count) or (this->columns_count != other.columns_count))
    {
        for (int i = 0; i < this->rows_count; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.rows_count];
        for (int i = 0; i < other.rows_count; i++)
        {
            matrix[i] = new double[other.columns_count];
        }
    }

    this->rows_count = other.rows_count;
    this->columns_count = other.columns_count;


    for (int i = 0; i < this->rows_count; i++)
    {
        for (int j = 0; j < this->columns_count; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
}


Matrix& Matrix::operator=(const Matrix& other)
{
    if ((this->rows_count != other.rows_count) or (this->columns_count != other.columns_count))
    {
        for (int i = 0; i < this->rows_count; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.rows_count];
        for (int i = 0; i < other.rows_count; i++)
        {
            matrix[i] = new double[other.columns_count];
        }
    }

    this->rows_count = other.rows_count;
    this->columns_count = other.columns_count;

    for (int i = 0; i < this->rows_count; i++) 
    {
        for (int j = 0; j < this->columns_count; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
    return *this;
}

Matrix::Matrix(const Matrix&& other) 
{
    if ((this->rows_count != other.rows_count) or (this->columns_count != other.columns_count))
    {
        for (int i = 0; i < this->rows_count; i++) 
        {
            delete matrix[i];
        }
        matrix = new double* [other.rows_count];
        for (int i = 0; i < other.rows_count; i++) 
        {
            matrix[i] = new double[other.columns_count];
        }
    }

    this->rows_count = other.rows_count;
    this->columns_count = other.columns_count;

    for (int i = 0; i < this->rows_count; i++) 
    {
        for (int j = 0; j < this->columns_count; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix&& other) 
{
    if ((this->rows_count != other.rows_count) or (this->columns_count != other.columns_count)) 
    {
        for (int i = 0; i < this->rows_count; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.rows_count];
        for (int i = 0; i < other.rows_count; i++) 
        {
            matrix[i] = new double[other.columns_count];
        }
    }

    this->rows_count = other.rows_count;
    this->columns_count = other.columns_count;

    for (int i = 0; i < this->rows_count; i++) 
    {
        for (int j = 0; j < this->columns_count; j++) 
        {
            matrix[i][j] = other[i][j];
        }
    }
    return *this;
}


MatrixRow Matrix::operator[](int i) const 
{
    return MatrixRow(matrix[i]);
}


Matrix operator+(Matrix const A, Matrix const B) 
{
    if ((A.columns() != B.columns()) or (A.rows() != B.rows())) 
    {
        std::cout << "Error in matix sum: " << A.columns() << " != " << B.columns() << " or " << A.rows() << " != " << B.rows() << std::endl;
        return Matrix(1, 1);
    }

    Matrix C(A.rows(), A.columns());

    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.columns(); j++)
        {
            C[i][j] = A[i][j] + B[i][j];
        }
    }
    return C;
}

Matrix operator-(Matrix const A, Matrix const B)
{
    if ((A.columns() != B.columns()) or (A.rows() != B.rows()))
    {
        std::cout << "Error in matix difference: " << A.columns() << " != " << B.columns() << " or " << A.rows() << " != " << B.rows() << std::endl;
        return Matrix(1, 1);
    }

    Matrix C(A.rows(), A.columns());

    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < A.columns(); j++) 
        {
            C[i][j] = A[i][j] - B[i][j];
        }
    }
    return C;
}

Matrix operator*(Matrix const A, Matrix const B)
{
    if (A.columns() != B.rows()) 
    {
        std::cout << "Error in matrix size" << std::endl;
        return Matrix(1, 1);
    }

    Matrix C(A.rows(), B.columns());

    for (int i = 0; i < A.rows(); i++)
    {
        for (int j = 0; j < B.columns(); j++) 
        {
            for (int k = 0; k < B.rows(); k++)
            {
                C[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    return C;
}

Matrix operator*(double const factor, Matrix const A)
{
    Matrix C(A.rows_count, A.columns_count);
    for (int i = 0; i < A.rows_count; i++) 
    {
        for (int j = 0; j < A.columns_count; j++)
        {
            C[i][j] = A[i][j] * factor;
        }
    }
    return C;
}

Matrix operator*(Matrix const A, double const vector[])
{
    Matrix C(A.rows_count, A.columns_count);
    for (int i = 0; i < A.rows_count; i++)
    {
        for (int j = 0; j < A.columns_count; j++)
        {
            C[i][j] = A[i][j] * vector[j];
        }
    }
    return C;
}


Matrix operator*(Matrix const A, double const factor)
{
    Matrix C(A.rows_count, A.columns_count);
    for (int i = 0; i < A.rows_count; i++) 
    {
        for (int j = 0; j < A.columns_count; j++) 
        {
            C[i][j] = A[i][j] * factor;
        }
    }
    return C;
}

std::ostream& operator<<(std::ostream& outstream, Matrix mtr)
{
    
    for (int i = 0; i < mtr.rows(); i++) 
    {
        for (int j = 0; j < mtr.columns(); j++) 
        {

            outstream << mtr[i][j] << " ";
        }
        outstream << std::endl;
    }
    return outstream;
}

Matrix::~Matrix()
{
    if (matrix != nullptr)
    {
        for (int i = 0; i < rows_count; i++)
        {
            delete[] matrix[i];
        }
    }
}