#ifndef __OCTAHEDRON_H__
#define __OCTAHEDRON_H__

#include <assert.h>
#include "shape.h"
#include "Polygon.h"

class octahedron : public shape
{
public:

	octahedron() : radius(1) { }
	void SetRadius(double d) { radius = d; }
	bool Intersect(const ray &R, intersection &I);
	double Radius(void) { return radius; }

private:
	
	double radius;// unscaled distance from the center to a vertex
};

#endif