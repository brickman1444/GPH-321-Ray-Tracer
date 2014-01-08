#ifndef __CUBE_H__
#define __CUBE_H__

#include <assert.h>
#include "shape.h"
#include "Polygon.h"

class cube : public shape
{
public:

	cube() : bottomBackLeftPoint(0,0,0), up(0,0,1), right(0,1,0), sideLength(1) { }
	void SetUp(vector u) { up = u; }
	void SetRight(vector r) { right = r; }
	void SetBottomBackLeftPoint(point bblp) { bottomBackLeftPoint = bblp; }
	void SetSideLength(double d) { sideLength = d; }
	void CalculateSides(void);
	bool Intersect(const ray &R, intersection &I);
	virtual void SetMaterial(material &m);
	virtual void SetMaterial(material *m);

	polygon side[6];// 0=leftSide; 1=rightSide; 2=upSide; 3=downSide; 4=forwardSide; 5=backSide;

private:
	
	point bottomBackLeftPoint;// The anchor point in the corner of the cube
	vector up, right;// vectors used to build out the cube from the corner. forward is constructed in CalculateSides()
	double sideLength;
};

#endif