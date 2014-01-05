	
#include "Cube.h"
#include "Polygon.h"
#include "material.h"

cube::cube(point p, vector u, vector r, double len) { 
	bottomBackLeftPoint = p; 
	up = u; 
	right = r; 
	sideLength = len; 
	CalculateSides();
}

void cube::CalculateSides() {
	// Create forward vector perpendicular to right and up
	// If right and up are the same direction or opposite directions, 
	// forward have 0 length. 
	vector forward = Cross(right, up);
	//recreate right vector perpendicular to up and forward in case the 
	//original right vector was not at a right angle to up
	vector right = Cross(up, forward);
	//normalize all vectors so they can be used for calculating coordinates
	right.Normalize();
	up.Normalize();
	forward.Normalize();

	//Left Side
	side[0].AddPoint(bottomBackLeftPoint);
	side[0].AddPoint(bottomBackLeftPoint + sideLength * forward);
	side[0].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * up);
	side[0].AddPoint(bottomBackLeftPoint + sideLength * up);
	side[0].CalculateNormal();

	//Right Side
	side[1].AddPoint(bottomBackLeftPoint + sideLength * right);
	side[1].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * right);
	side[1].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * up + sideLength * right);
	side[1].AddPoint(bottomBackLeftPoint + sideLength * up + sideLength * right);
	side[1].CalculateNormal();

	//Up Side
	side[2].AddPoint(bottomBackLeftPoint + sideLength * up);
	side[2].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * up);
	side[2].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * up + sideLength * right);
	side[2].AddPoint(bottomBackLeftPoint + sideLength * up + sideLength * right);
	side[2].CalculateNormal();

	//Down Side
	side[3].AddPoint(bottomBackLeftPoint);
	side[3].AddPoint(bottomBackLeftPoint + sideLength * forward);
	side[3].AddPoint(bottomBackLeftPoint + sideLength * forward + sideLength * right);
	side[3].AddPoint(bottomBackLeftPoint + sideLength * right);
	side[3].CalculateNormal();

	//Back Side
	side[4].AddPoint(bottomBackLeftPoint);
	side[4].AddPoint(bottomBackLeftPoint + sideLength * up);
	side[4].AddPoint(bottomBackLeftPoint + sideLength * up + sideLength * right);
	side[4].AddPoint(bottomBackLeftPoint + sideLength * right);
	side[4].CalculateNormal();

	//Forward Side
	side[5].AddPoint(bottomBackLeftPoint + sideLength * forward);
	side[5].AddPoint(bottomBackLeftPoint + sideLength * up + sideLength * forward);
	side[5].AddPoint(bottomBackLeftPoint + sideLength * up + sideLength * right + sideLength * forward);
	side[5].AddPoint(bottomBackLeftPoint + sideLength * right + sideLength * forward);
	side[5].CalculateNormal();
}

bool cube::Intersect(const ray &R, intersection &inter)
{
	//run the Intersect method for every side with different intersection objects
	//compare the t's of the intersection objects and set the cube's intersection object to the earliest t
	//return false if all intersects return false.

	// Make an impossibly large t so that any intersection would be before it
	inter.t = DBL_MAX;
	intersection temp = inter;
	bool anyIntersection = false;

	for (int i = 0; i < 6; i++) {
		if (side[i].Intersect(R, temp)) {
			anyIntersection = true;
			if (temp.t < inter.t) {
				inter = temp;
			}
		}
	}

	if (anyIntersection) {
		// Other data needed by the tracer
		inter.s = this;
		inter.diff = surfaceColor;
		inter.spec = specularColor;
		inter.shiny = shininess;
		return true;
	} else {
		return false;
	}
}
void cube::SetMaterial(material &m) {
	objectMat = &m;
	for (int i = 0; i < 5; i++)
		side[i].SetMaterial(&m);
}

void cube::SetMaterial(material *m) {
	objectMat = m;
	for (int i = 0; i < 5; i++)
		side[i].SetMaterial(m);
}
