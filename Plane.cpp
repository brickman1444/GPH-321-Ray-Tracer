	
#include "plane.h"

bool plane::Intersect(const ray &R, intersection &inter)
{
	// The normal should already be calculated and placed 
	// in the member "normal".
	//
	// Calculate the intersection of the
	// ray with the plane.
	// 
	// Don't forget to check to see if the t you calculate for the
	// ray is > 0.

	double denominator = Dot(R.v,normal);

	if (IsZero(denominator)) return false;

	double t = -1*Dot((R.p - p),normal) / denominator;

	if (t <= 0) return false;

	point intersectionPoint = R.PointOnRay(t);

	inter.p = intersectionPoint;
	inter.n = normal;
	inter.t = t;

	// Other data needed by the tracer
	inter.s = this;
	inter.diff = surfaceColor;
	inter.spec = specularColor;
	inter.shiny = shininess;

	return true;
}

void plane::SetNormal(vector N) {
	if (IsEqual(N.Length(),1.0)) {
		normal = N;
	} else {
		N.Normalize();
		normal = N;
	}
}