#ifndef __POINT_H__
#define __POINT_H__

#include <iostream>
#include <assert.h>
#include <math.h>
#include "MiscMath.h"

using namespace std;

class point
{
public:

	point(double x = 0.0, double y = 0.0, double z = 0.0);

	double *AsArray(void) { return p; }
	const double *AsArray(void) const { return p; }
	void Normalize(void);

	double &operator[](unsigned int i) { assert(i >= 0 && i <= 3); return p[i]; }
	double operator[](unsigned int i) const { assert(i >= 0 && i <= 3); return p[i]; }

private:

	double p[4];
};

////////////////////////////////////////////////////////////
// Global mathematical operations
////////////////////////////////////////////////////////////

bool IsZero(const point &p);
bool IsEqual(const point &p, const point &q);
bool IsNotEqual(const point &p, const point &q);
bool IsNormal(const point &p);

double Distance(const point &p, const point &q);

istream &operator>>(istream &input, point &v);
ostream &operator<<(ostream &output, const point &v);

#endif