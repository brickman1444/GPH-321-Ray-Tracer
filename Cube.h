#ifndef __CUBE_H__
#define __CUBE_H__

#include <assert.h>
#include "shape.h"
#include "Polygon.h"

class cube : public shape
{
public:

	cube(point p = point(0,0,0), vector u = vector(0,0,1), vector r = vector(0,1,0), double len = 1);
	void SetUp(vector u) { up = u; }
	void SetRight(vector r) { right = r; }
	void SetBottomBackLeftPoint(point bblp) { bottomBackLeftPoint = bblp; }
	void CalculateSides(void);
	bool Intersect(const ray &R, intersection &I);

private:
	
	vector up, right;
	polygon side[6];
	point bottomBackLeftPoint;
	double sideLength;
	// 0=leftSide; 1=rightSide; 2=upSide; 3=downSide; 4=forwardSide; 5=backSide;
};

#endif