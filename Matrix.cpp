
#include <iostream>
using namespace std;
#include <math.h>
#include <assert.h>
#include <memory.h>
#include "jmisc.h"
#include "matrix.h"

matrix::matrix(int r, int c, double diagVal)
{
	m = NULL;
	Resize(r, c, true);
	for (int i = 0; i < rows; i++)
		m[i][i] = diagVal;
}

matrix::matrix(int r, int c, double *vals)  
{
	m = NULL;
	Resize(r, c, true);

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{
			m[i][j] = vals[i * cols + j];
		}
	}
}

matrix::matrix(const matrix &m1)
{
	m = NULL;
	Resize(m1.rows, m1.cols, false);
	Copy(m1);
}

matrix::~matrix()
{
	delete [] m[0]; 
	delete [] m; 
}

matrix &matrix::operator=(const matrix &m1)
{
	Resize(m1.rows, m1.cols, false);
	Copy(m1);
	return *this;
}

void matrix::Resize(int r, int c, bool clearNewMem)
{
	if (m != NULL)
	{
		delete [] m[0];
		delete [] m;
	}

	// Allocate matrix memory
	rows = r; cols = c;
	m = new double*[rows];
	assert(m != NULL);

	// Allocate row array
	m[0] = new double[rows*cols];
	assert(m[0] != NULL);

	// Fill row array with pointers to rows
	for (int i = 1; i < rows; i++)
		m[i] = m[i-1] + cols;

	// Erase memory if requested
	if (clearNewMem)
		memset(m[0], 0, rows*cols*sizeof(double));
}

void matrix::Copy(const matrix &m1)
{
	assert(rows == m1.rows && cols == m1.cols);

	// A faster way to do this is using a memcpy.  It is similar
	// to memset and if you want to know about it please come see me.
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
		{	
			m[i][j] = m1.m[i][j];
		}
	}
}

void matrix::Print(int nPlaces) const
{
	std::streamsize oldP = cout.precision();
	cout.precision(nPlaces);
	for (int i = 0; i < rows; i++)
	{
		for (int j = 0; j < cols; j++)
			cout << m[i][j] << "\t";
		cout << endl;
	}
	cout.precision(oldP);
}

matrix matrix::Transpose()
{
	matrix result (cols, rows);
	// to do: return a matrix that is the transpose of m, i.e.,
	// result[i][j] = m[j][i]

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			result[i][j] = m[j][i];
		}
	}

	return result;
}

void matrix::MakeRotationX(double alpha)
{
	alpha *= PI/180;
	double mData[] = {1,0,0,0,
					  0,cos(alpha),-1*sin(alpha),0,
					  0,sin(alpha),cos(alpha),0,
					  0,0,0,1};
	(*this) = matrix(4, 4, mData);

	// to do:  set up a matrix that rotates by an angle alpha about the x-axis
}

void matrix::MakeRotationY(double alpha)
{
	alpha *= PI/180;
	double mData[] = {cos(alpha),0,sin(alpha),0,
					  0,1,0,0,
					  -1*sin(alpha),0,cos(alpha),0,
					  0,0,0,1};
	(*this) = matrix(4, 4, mData);
	// to do:  set up a matrix that rotates by an angle alpha about the y-axis
}

void matrix::MakeRotationZ(double alpha)
{
	alpha *= PI/180;
	double mData[] = {cos(alpha),-1*sin(alpha),0,0,
					  sin(alpha),cos(alpha),0,0,
					  0,0,1,0,
					  0,0,0,1};
	(*this) = matrix(4, 4, mData);
	// to do:  set up a matrix that rotates by an angle alpha about the z-axis
}

void matrix::MakeRotation(double alpha, const vector &v)
{
	(*this) = matrix(4, 4, 1.0);

	// to do:  set up matrix that rotates about
	// an arbitrary axis v by an angle alpha
}

void matrix::MakeScale(double x, double y, double z)
{
	double mData[] = {x,0,0,0,
					  0,y,0,0,
					  0,0,z,0,
					  0,0,0,1};
	(*this) = matrix(4, 4, mData);
    // to do:  set up a matrix that scales by a factor x along the x-axis, 
	//                              scales by a factor y along the y-axis,
	//                          and scales by a factor z along the z-axis
}

void matrix::MakeTranslation(double x, double y, double z)
{
	double mData[] = {1,0,0,x,
					  0,1,0,y,
					  0,0,1,z,
					  0,0,0,1};
	(*this) = matrix(4, 4, mData);

	// to do:  set up a matrix that translates by (x, y, z)
}

matrix matrix::Identity(int size) { return matrix(size, size, 1.0); }
matrix matrix::Zero(int size) { return matrix(size, size, 0.0); }
matrix matrix::RotationX(double alpha) { matrix m(4, 4); m.MakeRotationX(alpha); return m; }
matrix matrix::RotationY(double alpha) { matrix m(4, 4); m.MakeRotationY(alpha); return m; }
matrix matrix::RotationZ(double alpha) { matrix m(4, 4); m.MakeRotationZ(alpha); return m; }
matrix matrix::Rotation(double alpha, const vector &v) { matrix m(4, 4); m.MakeRotation(alpha, v); return m; }
matrix matrix::Scale(double x, double y, double z) { matrix m(4, 4); m.MakeScale(x, y, z); return m; }
matrix matrix::Translation(double x, double y, double z) { matrix m(4, 4); m.MakeTranslation(x, y, z); return m; }

// For these problems note that they may not be 3x3's or 4x4's 
// or even square matrices, so your functions should work no matter
// what the number of rows and columns in the matrices are!
bool IsZero(const matrix &m)
{
	// to do:  Check that all elements of m
	// are (floating point) equal to zero

	for (int i = 0; i < m.Rows(); i++) {
		for (int j = 0; j < m.Cols(); j++) {
			if (IsNotEqual(m[i][j],0) )
				return false;
		}
	}

	return true;
}

bool IsIdentity(const matrix &m)
{
    // to do:  Check that all of the diagonal
	// elements are (floating-point) equal to 1
	// and the nondiagonal elements are (floating-point)
	// equal to zero

	for (int i = 0; i < m.Rows(); i++) {
		for (int j = 0; j < m.Cols(); j++) {
			if (i == j) {//integer equals
				if (IsNotEqual(m[i][j],1) ){//if on the diagonal, must be 1
					return false;
				}
			} else {
				if (IsNotEqual(m[i][j],0) ){//else must be 0
					return false;
				}
			}
		}
	}

	return true;
}

bool IsEqual(const matrix &m1, const matrix &m2)
{
	if ((m1.Rows() != m2.Rows()) || (m1.Cols() != m2.Cols()))
		return false;

	// to do:  check that each element of m1 is equal to
	// the corresponding element of m2
	for (int i = 0; i < m1.Rows(); i++) {
		for (int j = 0; j < m1.Cols(); j++) {
			if (IsNotEqual(m1[i][j],m2[i][j])) {
				return false;
			}
		}
	}

	return true;
}

bool IsNotEqual(const matrix &m1, const matrix &m2)
{
	return !IsEqual(m1, m2);
}

matrix matrix::operator*=(const matrix &m1)
{
	matrix tmp(rows, m1.cols);
	*this = tmp * m1;
	return *this;
}

matrix operator+(const matrix &m1, const matrix &m2)
{
	assert(m1.Rows() == m2.Rows() && m1.Cols() == m2.Cols());

	matrix result(m1.Rows(), m1.Cols());
	// to do:  Add matrices m1 and m2 to yield result
	// result = m1 + m2
	return result;
}

matrix operator-(const matrix &m1, const matrix &m2)
{
	assert(m1.Rows() == m2.Rows() && m1.Cols() == m2.Cols());

	matrix result(m1.Rows(), m1.Cols());
	// to do:  subtract matrix m2 from matrix m1 to yield result:
	// result = m1 - m2
	return result;
}

matrix operator*(const matrix &m1, const matrix &m2)
{
	assert(m1.Cols() == m2.Rows());

	matrix result(m1.Rows(), m2.Cols());
	// todo: multiply two matrices to set values for result. 
	// result = m1 * m2
	for (int i = 0; i < m1.Rows(); i++) {
		for (int j = 0; j < m2.Cols(); j++) {
			result[i][j] = 0;

			for (int k = 0; k < m1.Cols(); k++) {
				result[i][j] += m1[i][k]*m2[k][j];
			}
		}
	}

	return result;
}

point operator*(const matrix &m, const point &p)
{
	assert((m.Rows() == 3 && m.Cols() == 3) || (m.Rows() == 4 && m.Cols() == 4));

	// to do: multiply the matrix and point and return the result
	// result = m * p
	point result = point(0,0,0);
	for (int i = 0; i < m.Rows(); i++) {
		result[i] = 0;
		for (int j = 0; j < m.Cols(); j++) {
			result[i] += m[i][j]*p[j];
		}
	}

	return result;
}

vector operator*(const matrix &m, const vector &v)
{
	assert((m.Rows() == 3 && m.Cols() == 3) || (m.Rows() == 4 && m.Cols() == 4));
	// to do: multiply the matrix and vector and return the result	
	// result = m * v
	vector result = vector(0,0,0);
	for (int i = 0; i < m.Rows(); i++) {
		result[i] = 0;
		for (int j = 0; j < m.Cols(); j++) {
			result[i] += m[i][j]*v[j];
		}
	}

	return result;
}

///////////////////////////////////////////////////////////////////////////
// Advanced methods for doing Gauss-Jordan elimination and inverses
///////////////////////////////////////////////////////////////////////////

void matrix::SwapRows(int i, int j)
{
	// to do: swap rows i and j of the matrix 
	double* tempArray = new double[cols];

	//Three way switch
	// 1) store i in tempArray
	for (int x = 0; x < cols; x++) {
		tempArray[x] = m[i][x];
	}
	// 2) store j in i
	for (int x = 0; x < cols; x++) {
		m[i][x] = m[j][x];
	}
	// 3) store tempArray in j
	for (int x = 0; x < cols; x++) {
		m[j][x] = tempArray[x];
	}

	delete tempArray;
}

void matrix::AddRowMultiple(double d, int i, int j, int startCol)
{
	// to do:  multiply row i by factor d, 
	//         and add to row j
	for (int x = startCol; x < cols; x++) {
		m[j][x] += d*m[i][x];
	}
}

void matrix::MultiplyRow(double d, int i)
{
	// to do:  multiply row i by factor d 
	for (int x = 0; x < cols; x++) {
		m[i][x] *= d;
	}
}

// For the determinant, we only have to eliminate
// elements below diagonal
double matrix::Determinant(void)
{
	assert(rows == cols);

    // totally beyond the scope of the course --
	// compute and return the determinant
	return 1;
}

bool matrix::GaussJordanUnstable(void)
{
	for (int j = 0; j < min(rows, cols); j++)
	{
		if (IsZero(m[j][j]))
			return false;

		MultiplyRow(1.0 / m[j][j], j);
		for (int i = 0; i < rows; i++)
		{
			if (j != i)
				AddRowMultiple(-m[i][j], j, i, j);
		}
	}
	return true;
}

bool matrix::GaussJordan(void)
{
	// for each row i
	for (int i = 0; i < min(rows, cols); i++)
	{
		//let pivotVal be the dialogonal value
		double pivotVal = m[i][i];
		//initialize pivotIndex to the current row
		int pivotIndex = i;

		//for each row j where j > i
		for (int j = i + 1; j < rows; j++) {
			//if the absolute value of the diagonal value of row i is greater than
			//the absolute value of the pivotVal then update pivotVal and update pivotIndex
			if (fabs(m[j][i]) > fabs(pivotVal)) {
				pivotVal = m[j][i];
				pivotIndex = j;
			}
		}

		//if pivotVal is zero, return false
		if (IsZero(pivotVal))
			return false;
		//otherwise swap rows pivotIndex and i
		SwapRows(pivotIndex, i);

		//divide ith row by pivotVal
		MultiplyRow(1.0 / m[i][i], i);

		//use the ith row to annihilate nonzero entries in the ith column of all other rows
		for (int j = 0; j < rows; j++)
		{
			if (i != j)
				AddRowMultiple(-m[j][i], i, j, i);
		}
	}
	return true;
}

matrix matrix::SubMatrix(int rStart, int cStart, int nRows, int nCols)
{
	matrix result(nRows, nCols);
	for (int i = 0; i < nRows; i++)
		for (int j = 0; j < nCols; j++)
			result.m[i][j] = m[i+rStart][j+cStart];

	return result;
}

bool matrix::Solve(matrix &result, const matrix &RHS) // Use Gauss-Jordan to solve
{
	assert(rows == RHS.rows); // The RHS must be compatible

	matrix temp = Augment(*this, RHS);
	if (temp.GaussJordan())
	{
		result = temp.SubMatrix(0, cols, RHS.rows, RHS.cols);
		return true;
	}
	else
	{
		result = temp;
		return false;
	}
}

// This doesn't work in general anyway, so don't try to 
// deal with degenerate cases.  Exists to demonstrate failure
matrix matrix::SolveUnstable(const matrix &RHS) 
{
	assert(rows == RHS.rows);  
	matrix temp = Augment(*this, RHS);
	temp.GaussJordanUnstable();
	return temp.SubMatrix(0, cols, RHS.rows, RHS.cols);
}

bool matrix::Inverse(matrix &inv)
{
	assert(rows == cols); // must be square

	matrix identity(rows, cols, 1.0);
	inv.Resize(rows, cols);
	return Solve(inv, identity);
}

matrix matrix::InverseUnstable()
{
	assert(rows == cols); // must be square
	return SolveUnstable(matrix(rows, cols, 1.0));
}

matrix matrix::Augment(const matrix &m1, const matrix &m2)
{
	assert(m1.rows == m2.rows);

	int i, j;
	int rows = m1.rows;
	int cols = m1.cols + m2.cols;
	matrix result(rows, cols);

	for (i = 0; i < rows; i++) {
		for (j = 0; j < m1.cols; j++)
			result.m[i][j] = m1.m[i][j];
		for (j = 0; j < m2.cols; j++)
			result.m[i][j+m1.cols] = m2.m[i][j];
	}
	return result;
}

