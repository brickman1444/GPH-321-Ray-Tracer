#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "point.h"
#include "vector.h"
#include <stdarg.h>

class matrix
{
public:

	matrix(int r = 4, int c = 4, double diagVal = 1.0);
	matrix(int r, int c, double *vals);
	matrix(const matrix &m1);
	~matrix();

	void Resize(int r, int c, bool clearNewMem = true);

	matrix &operator=(const matrix &m1);

	int Rows(void) const { return rows; }
	int Cols(void) const { return cols; }

	double *operator[](int i) { assert(i >= 0 && i < rows); return m[i]; }
	const double *operator[](int i) const { assert(i >= 0 && i < rows); return m[i]; }

	matrix operator *=(const matrix &m1);

	void Print(int nPlaces = 3) const;

	double Determinant(void);

	bool GaussJordanUnstable(void);
	bool GaussJordan(void);
	void SwapRows(int i, int j);
	void AddRowMultiple(double d, int i, int j, int startCol = 0);
	void MultiplyRow(double d, int i);
	bool Solve(matrix &result, const matrix &RHS);
	bool Inverse(matrix &inv);

	matrix SolveUnstable(const matrix &RHS);
	matrix SubMatrix(int rStart, int cStart, int nRows, int nCols);
	matrix InverseUnstable();
	matrix Transpose();

	void MakeZero(void) { memset(m[0], 0, rows*cols*sizeof(double)); }
	void MakeIdentity(int size) { (*this) = matrix(size, size, 1.0); }
	void MakeRotationX(double alpha);
	void MakeRotationY(double alpha);
	void MakeRotationZ(double alpha);
	void MakeRotation(double alpha, const vector &v);
	void MakeScale(double x, double y, double z);
	void MakeTranslation(double x, double y, double z);

	// These are the static matrix methods that allow you to construct matrices without an explicit object
	//
	// e.g. matrix m = matrix::RotationZ(30) * matrix::rotationY(20);
	//
	static matrix Identity(int size);
	static matrix Zero(int size);
	static matrix RotationX(double alpha);
	static matrix RotationY(double alpha);
	static matrix RotationZ(double alpha);
	static matrix Rotation(double alpha, const vector &v);
	static matrix Scale(double x, double y, double z);
	static matrix Translation(double x, double y, double z);

private:

	// This method is private because it is restricted to working with 
	// equally sized matrices.  It is called by operator= and the copy constructor
	void Copy(const matrix &m1);

	// This static method is used by the "Solve" and "Inverse"
	// methods to build the combined matrix to reduce with 
	// Gauss-Jordan
	static matrix Augment(const matrix &m1, const matrix &m2);

	int rows, cols;
	double **m;
};

bool IsZero(const matrix &m);
bool IsIdentity(const matrix &m);
bool IsEqual(const matrix &m1, const matrix &m2);
bool IsNotEqual(const matrix &m1, const matrix &m2);

matrix operator+(const matrix &m1, const matrix &m2);
matrix operator-(const matrix &m1, const matrix &m2);
matrix operator*(const matrix &m1, const matrix &m2);
point operator*(const matrix &m, const point &v);
vector operator*(const matrix &m, const vector &v);


#endif