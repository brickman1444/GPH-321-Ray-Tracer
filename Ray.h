#ifndef __RAY_H__
#define __RAY_H__

#include "point.h"
#include "vector.h"

class ray
{
public:

	point PointOnRay(double t) const { return p + t*v; }

	point p;
	vector v;
};

#endif