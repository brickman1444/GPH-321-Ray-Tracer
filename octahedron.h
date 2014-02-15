#ifndef __OCTAHEDRON_H__
#define __OCTAHEDRON_H__

#include <assert.h>
#include "shape.h"
#include "Polygon.h"

class octahedron : public shape
{
public:

	octahedron() { }
	bool Intersect(const ray &R, intersection &I);
};

#endif