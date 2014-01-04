	
#include "polygon.h"

bool polygon::Intersect(const ray &R, intersection &inter)
{
	// To Do: If you have implemented the "CalculateNormal" method 
	// below, then the normal should already be calculated and placed 
	// in the member "n".
	//
	// Now, using n, you need to calculate the intersection of the
	// ray with the plane containing the polygon.  Then, once you 
	// have that point, you need to loop through each edge in the
	// polygon and make sure the point is to the left of that edge.
	//
	// If it is to the left of every edge, then fill the structure
	// inter with all of the data for the intersection and return 
	// true, if not, return 0. 
	// 
	// Don't forget to check to see if the t you calculate for the
	// ray is > 0.

	double denominator = Dot(R.v,n);

	if (IsZero(denominator)) return false;

	double t = -1*Dot((R.p - p[0]),n) / denominator;

	if (t <= 0) return false;

	point intersectionPoint = R.PointOnRay(t);

	for (int i = 0; i < nPoints; i++) {
		if (Dot(intersectionPoint - p[i], Cross(n, (p[(i+1)%nPoints]-p[i]))) < 0)
			return false;
	}

	inter.p = intersectionPoint;
	inter.n = n;
	inter.t = t;


	// Other data needed by the tracer
	inter.s = this;
	inter.diff = surfaceColor;
	inter.spec = specularColor;
	inter.shiny = shininess;

	return true;
}

void polygon::CalculateNormal(void)
{
	if (nPoints < 3)
		return;

	// To Do: Use vectors from p[0] to neighboring points on the 
	// polygon to calculate the polygon's normal. You should be 
	// careful to take care of the case where vectors give you a 
	// zero normal.  Normalize the normal and place it in the member 
	// "n" so that the intersection method can use it when called.
	// 
	// Notice that this method has to be called manually when you are
	// building a polygon, after you have added all the points.  Look
	// in Application.cpp for an example.

	int counter = 0;
	do {
		n = Cross(p[counter + 1] - p[0], p[counter + 2] - p[0]);
		counter++;
	} while (IsZero(n.Length()) && counter < nPoints -2);
	n.Normalize();
}