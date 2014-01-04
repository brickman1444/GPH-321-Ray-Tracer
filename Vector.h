#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "point.h"

// Explanations of certain methods may be found in point.h

class vector
{
public:

	// Define a constructor to initialize points
	// Notice that the default parameters allow this 
	// constructor to act as the default constructor.
	//
	// So by default a vector is initialized to <0, 0, 0>;
	vector(double x = 0, double y = 0.0, double z = 0.0);

	double Length(void) const;
	void Normalize(void);       // Make the vector a unit vector

	vector &operator+=(const vector &w);
	vector &operator-=(const vector &w);
	vector &operator*=(double d);
	vector &operator/=(double d);

	// Calculates the projection onto w and perpendicular component
	vector Proj(const vector &w);
	vector Perp(const vector &w);

	// Warning.  This method gives the possiblity of corrupting v[3]!
	double *AsArray(void) { return v; }
	const double *AsArray(void) const { return v; }

	double &operator[](unsigned int i) { assert(i >= 0 && i <= 3); return v[i]; }
	double operator[](unsigned int i) const { assert(i >= 0 && i <= 3); return v[i]; }

private:
	
	double v[4];
};

////////////////////////////////////////////////////////////
// Global mathematical operations
////////////////////////////////////////////////////////////

bool IsZero(const vector &v);
bool IsEqual(const vector &v, const vector &w);
bool IsNotEqual(const vector &v, const vector &w);

double Dot(const vector &v, const vector &w);
vector Cross(const vector &v, const vector &w);

// Adding and Subtracting
vector operator+(const vector &v, const vector &w);
vector operator-(const vector &v, const vector &w);

// Multiplying and dividing by scalars
vector operator*(double d, const vector &v); // We need both of these, because we
vector operator*(const vector &v, double d); // want to say both d*v and v*d!
vector operator/(const vector &v, double d); // Note that (d / v) doesn't make sense.

// Negating a vector
vector operator-(const vector &v);

// Arithmetic operations involving points
point operator+(const point &p, const vector &v);
point operator+(const vector &v, const point &p);
point &operator+=(point &p, const vector &v);

point operator-(const point &p, const vector &v);
point &operator-=(point &p, const vector &v);

vector operator-(const point &p, const point &q);

istream &operator>>(istream &input, vector &v);
ostream &operator<<(ostream &output, const vector &v);

#endif