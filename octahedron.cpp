#include "octahedron.h"

bool octahedron::Intersect(const ray &R, intersection &inter)
{
	// An octahedron is just a sphere in manhattan distance, so
	// abs(R.v[0]*t R.p[0]) + abs(R.v[0]*t R.p[0]) + abs(R.v[0]*t R.p[0]) = radius
	// there are eight conditions because each absolute value can be positive or negative 
	// and 2^3 = 8. This makes sense because the octahedron has eight sides

	// Iterate through the sides of the octahedron and check to see if the ray basically 
	// intersects with the plane defined by the sides. Save the t (time) value to an 8 value 
	// array of doubles

	double t[8];

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
		t[3] = (radius - R.p[0] + R.p[1] + R.p[2]) / (R.v[0] - R.v[1] - R.v[2]);
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

	// Set t in inter, which stores the final result, to an impossibly large value so that
	// if any intersection is detected, the t value of that intersection will be less than 
	// inter.t and the actual intersection data will overwrite the default inter data.
	inter.t = DBL_MAX;

	// Boolean for if any intersection has been found. Defaults to false and is set to true 
	// once an intersection has been found.
	bool found = false;

	// Iterate through the array to check which t to use
	for (int i = 0; i < 8; i++) {

		// t[i] must be greater than 0 to be past the near clip plane which is right at the camera
		// if t[i] is less than the inter.t then the shape hit at t[i] is in front of the shape  
		// currently stored in inter
		if (t[i] > 0 && t[i] < inter.t) {
			// Get the point in 3D space
			inter.p = R.PointOnRay(t[i]);

			// Check whether the point in space is radius units away from the origin in 
			// manhattan distance.
			if (IsEqual(fabs(inter.p[0]) + fabs(inter.p[1]) + fabs(inter.p[2]),radius)) {
				found = true;
				inter.t = t[i];

				// Assign the normal based on which side the point is on.
				switch (i) {
					case 0: inter.n = vector(1,1,1); break;
					case 1: inter.n = vector(1,1,-1); break;
					case 2: inter.n = vector(1,-1,1); break;
					case 3: inter.n = vector(1,-1,-1); break;
					case 4: inter.n = vector(-1,1,1); break;
					case 5: inter.n = vector(-1,1,-1); break;
					case 6: inter.n = vector(-1,-1,1); break;
					case 7: inter.n = vector(-1,-1,-1); break;
				}
			}
		}
	}

	if (!found) {
		return false;
	}

	// Other data needed by the tracer
	inter.s = this;
	inter.diff = surfaceColor;
	inter.spec = specularColor;
	inter.shiny = shininess;

	return true;
}