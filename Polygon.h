#ifndef __POLYGON_H__
#define __POLYGON_H__

#include <assert.h>
#include "shape.h"

class polygon : public shape
{
public:

	polygon() { nPoints = 0; }
	
	bool Intersect(const ray &R, intersection &I);
	void AddPoint(point P) { p[nPoints] = P; nPoints++; }
	void SetPoint(point P, int i) { assert(i < nPoints); p[i] = P; }
	void CalculateNormal(void);
	point *AsArray(void) { return p; }
	int NPoints(void) { return nPoints; }

	point &operator[](int i) { assert(i < nPoints); return p[i]; }
	const point &operator[](int i) const { assert(i < nPoints); return p[i]; }

private:

	point p[50];
	int nPoints;
	vector n;
};

#endif