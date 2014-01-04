#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "shape.h"

class sphere : public shape
{
public:

	sphere() : center(0, 0, 0), radius(1.0) { }

	void SetCenter(const point &p) { center = p; }
	void SetRadius(double r) { radius = r; }
	const point &Center(void) { return center; }
	double Radius(void) { return radius; }

	virtual bool Intersect(const ray &R, intersection &I);

private:
	point center;
	double radius;
};

#endif