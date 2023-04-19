#include "Matrix.h"

Matrix::Matrix(int n, int m) 
{
    this->matrix = new double* [n];
    this->n = n;
    this->m = m;
    for (int i = 0; i < n; i++)
    {
        this->matrix[i] = new double[m];
        for (int j = 0; j < m; j++)
        {
            this->matrix[i][j] = 0;
        }
    }
}

Matrix::Matrix(int n, int m, std::vector<std::vector<double>> vector_matrix)
{
    this->matrix = new double* [n];
    for (int i = 0; i < n; i++) 
    {
        this->matrix[i] = new double[m];
    }

    if (n == vector_matrix.size())
    {
        for (int i = 0; i < n; i++) 
        {
            if (vector_matrix[i].size() == m)
            {
                for (int j = 0; j < m; j++)
                {
                    this->matrix[i][j] = vector_matrix[i][j];
                }
            }
            else 
            {
                std::cout<<"Error in matrix constructor by vector :: error of row size!"<<std::endl;
                return;
            }
        }
        this->n = n;
        this->m = m;
    }
    else
    {
        std::cout << "Error in matrix constructor by vector :: error of columns size!" << std::endl;
    }
}

bool Matrix::is_empty()
{
    return matrix == nullptr;
}

int Matrix::rows() const
{
    return n;
}

int Matrix::columns() const
{
    return m;
}



void Matrix::make_identity()
{
    int min_dimension = std::min(n, m);
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

Matrix Matrix::transpose()
{
    Matrix transposed_matrix(m, n);
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            transposed_matrix[j][i] = this->matrix[i][j];
        }
    }
    return transposed_matrix;
}


Matrix::Matrix(const Matrix& other)
{
    if ((this->n != other.n) or (this->m != other.m))
    {
        for (int i = 0; i < this->n; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.n];
        for (int i = 0; i < other.n; i++)
        {
            matrix[i] = new double[other.m];
        }
    }

    this->n = other.n;
    this->m = other.m;


    for (int i = 0; i < this->n; i++)
    {
        for (int j = 0; j < this->m; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
}


Matrix& Matrix::operator=(const Matrix& other)
{
    if ((this->n != other.n) or (this->m != other.m))
    {
        for (int i = 0; i < this->n; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.n];
        for (int i = 0; i < other.n; i++)
        {
            matrix[i] = new double[other.m];
        }
    }

    this->n = other.n;
    this->m = other.m;

    for (int i = 0; i < this->n; i++) 
    {
        for (int j = 0; j < this->m; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
    return *this;
}

Matrix::Matrix(const Matrix&& other) 
{
    if ((this->n != other.n) or (this->m != other.m))
    {
        for (int i = 0; i < this->n; i++) 
        {
            delete matrix[i];
        }
        matrix = new double* [other.n];
        for (int i = 0; i < other.n; i++) 
        {
            matrix[i] = new double[other.m];
        }
    }

    this->n = other.n;
    this->m = other.m;

    for (int i = 0; i < this->n; i++) 
    {
        for (int j = 0; j < this->m; j++)
        {
            matrix[i][j] = other[i][j];
        }
    }
}

Matrix& Matrix::operator=(const Matrix&& other) 
{
    if ((this->n != other.n) or (this->m != other.m)) 
    {
        for (int i = 0; i < this->n; i++)
        {
            delete matrix[i];
        }
        matrix = new double* [other.n];
        for (int i = 0; i < other.n; i++) 
        {
            matrix[i] = new double[other.m];
        }
    }

    this->n = other.n;
    this->m = other.m;

    for (int i = 0; i < this->n; i++) 
    {
        for (int j = 0; j < this->m; j++) 
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

Matrix operator*(double const k, Matrix const A)
{
    Matrix C(A.n, A.m);
    for (int i = 0; i < A.n; i++) 
    {
        for (int j = 0; j < A.m; j++)
        {
            C[i][j] = A[i][j] * k;
        }
    }
    return C;
}

Matrix operator*(Matrix const A, double const k) 
{
    Matrix C(A.n, A.m);
    for (int i = 0; i < A.n; i++) 
    {
        for (int j = 0; j < A.m; j++) 
        {
            C[i][j] = A[i][j] * k;
        }
    }
    return C;
}

std::ostream& operator<<(std::ostream& strm, Matrix mtr)
{
    for (int i = 0; i < mtr.rows(); i++) 
    {
        for (int j = 0; j < mtr.columns(); j++) 
        {

            strm << mtr[i][j] << " ";
        }
        strm << "\n";
    }
    return strm;
}

Matrix::~Matrix()
{
    if (matrix != nullptr)
    {
        for (int i = 0; i < n; i++)
        {
            delete[] matrix[i];
        }
    }
}