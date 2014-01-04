#ifndef __PLANE_H__
#define __PLANE_H__

#include <assert.h>
#include "shape.h"

class plane : public shape
{
public:

	plane(point P = point(0,0,0), vector N = vector(0,0,1)) { p = P; normal = N; normal.Normalize(); }
	bool Intersect(const ray &R, intersection &I);
	void SetPoint(point P) { p = P; }
	void SetNormal(vector N);

private:

	point p;
	vector normal;
};

#endif