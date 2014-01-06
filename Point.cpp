#pragma warning(disable : 4996) // Turns off secure_crt warnings in VS 2005 and up

#include "point.h"
#include <math.h>
#include <jmisc.h> // Miscellaneous routines, including sqr()

point::point(double x, double y, double z)
{
	p[0] = x;
	p[1] = y;
	p[2] = z;
	p[3] = 1;
}

void point::Normalize(void)
{
	assert(IsNotEqual(p[3],0));
	p[0] /= p[3];
	p[1] /= p[3];
	p[2] /= p[3];
	p[3] = 1;
}

bool IsZero(const point &p)
{
	// To Do
	//
	// Return true if the point p is the origin (0, 0, 0).
	return IsEqual(p[0],0) && IsEqual(p[1],0) && IsEqual(p[2],0);
}

bool IsEqual(const point &p, const point &q)
{
	// To Do 
	//
	// Return true if the two points are equal
	return IsEqual(p[0],q[0]) && IsEqual(p[1],q[1]) && IsEqual(p[2],q[2]);
}

bool IsNotEqual(const point &p, const point &q) 
{ 
	return !IsEqual(p, q); 
}

bool IsNormal(const point &p)
{
	return IsEqual(p[3],1);
}

double Distance(const point &p, const point &q)
{
	// Calculate the distance between the two points
	return sqrt(sqr(p[0] - q[0])+sqr(p[1] - q[1])+sqr(p[2] - q[2]));
}

istream &operator>>(istream &input, point &p)
{
	input >> p[0] >> p[1] >> p[2];
	return input;
}

ostream &operator<<(ostream &output, const point &p)
{
	output << p[0] << " " << p[1] << " " << p[2];
	return output;
}

