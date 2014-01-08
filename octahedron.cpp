#include "octahedron.h"

bool octahedron::Intersect(const ray &R, intersection &inter)
{
	double t[8];

	// abs(R.v[0]*t R.p[0]) + abs(R.v[0]*t R.p[0]) + abs(R.v[0]*t R.p[0]) = radius
	// there are eight conditions because each absolute value can be positive or negative 
	// and 2^3 = 8. This makes sense because the octahedron has eight sides

	//1
	if (!IsZero(R.v[0] + R.v[1] + R.v[2])) {
		t[0] = (radius - R.p[0] - R.p[1] - R.p[2]) / (R.v[0] + R.v[1] + R.v[2]);
	}
	//2
	if (!IsZero(R.v[0] + R.v[1] - R.v[2])) {
		t[1] = (radius - R.p[0] - R.p[1] + R.p[2]) / (R.v[0] + R.v[1] - R.v[2]);
	}
	//3
	if (!IsZero(R.v[0] - R.v[1] + R.v[2])) {
		t[2] = (radius - R.p[0] + R.p[1] - R.p[2]) / (R.v[0] - R.v[1] + R.v[2]);
	}
	//4
	if (!IsZero(R.v[0] - R.v[1] - R.v[2])) {
		t[3] = (radius + R.p[0] + R.p[1] + R.p[2]) / (R.v[0] - R.v[1] - R.v[2]);
	}
	//5
	if (!IsZero(-R.v[0] + R.v[1] + R.v[2])) {
		t[4] = (radius + R.p[0] - R.p[1] - R.p[2]) / (-R.v[0] + R.v[1] + R.v[2]);
	}
	//6
	if (!IsZero(-R.v[0] + R.v[1] - R.v[2])) {
		t[5] = (radius + R.p[0] - R.p[1] + R.p[2]) / (-R.v[0] + R.v[1] - R.v[2]);
	}
	//7
	if (!IsZero(-R.v[0] - R.v[1] + R.v[2])) {
		t[6] = (radius + R.p[0] + R.p[1] - R.p[2]) / (-R.v[0] - R.v[1] + R.v[2]);
	}
	//8
	if (!IsZero(-R.v[0] - R.v[1] - R.v[2])) {
		t[7] = (radius + R.p[0] + R.p[1] + R.p[2]) / (-R.v[0] - R.v[1] - R.v[2]);
	}

	inter.t = DBL_MAX;
	bool found = false;

	for (int i = 0; i < 8; i++) {
		if (t[i] > 0 && t[i] < inter.t) {
			found = true;
			inter.t = t[i];
		}
	}

	if (!found) {
		return false;
	}

	// Point and normal of intersection
	inter.p = R.PointOnRay(inter.t);
	inter.n = inter.p - point(0,0,0);

	// Other data needed by the tracer
	inter.s = this;
	inter.diff = surfaceColor;
	inter.spec = specularColor;
	inter.shiny = shininess;

	return true;
}