	
#include "Cube.h"
#include "Polygon.h"
#include "material.h"

const vector cube::up = vector(0,0,1);
const vector cube::forward = vector(1,0,0);
const vector cube::right = vector(0,1,0);
const point cube::originPoint = point(0,0,0);

void cube::CalculateSides() {

	// Construct the polygon sides of the cube

	//Left Side
	side[0].AddPoint(originPoint);
	side[0].AddPoint(originPoint + forward);
	side[0].AddPoint(originPoint + forward + up);
	side[0].AddPoint(originPoint + up);
	side[0].CalculateNormal();

	//Right Side
	side[1].AddPoint(originPoint + right);
	side[1].AddPoint(originPoint + right + forward);
	side[1].AddPoint(originPoint + right + forward + up);
	side[1].AddPoint(originPoint + right + up);
	side[1].CalculateNormal();

	//Up Side
	side[2].AddPoint(originPoint + up);
	side[2].AddPoint(originPoint + up + forward);
	side[2].AddPoint(originPoint + up + forward + right);
	side[2].AddPoint(originPoint + up + right);
	side[2].CalculateNormal();

	//Down Side
	side[3].AddPoint(originPoint);
	side[3].AddPoint(originPoint + forward);
	side[3].AddPoint(originPoint + forward + right);
	side[3].AddPoint(originPoint + right);
	side[3].CalculateNormal();

	//Back Side
	side[4].AddPoint(originPoint);
	side[4].AddPoint(originPoint + up);
	side[4].AddPoint(originPoint + up + right);
	side[4].AddPoint(originPoint + right);
	side[4].CalculateNormal();

	//Forward Side
	side[5].AddPoint(originPoint + forward);
	side[5].AddPoint(originPoint + forward + up);
	side[5].AddPoint(originPoint + forward + up + right);
	side[5].AddPoint(originPoint + forward + right);
	side[5].CalculateNormal();
}

bool cube::Intersect(const ray &R, intersection &inter)
{
	// Make an impossibly large t so that any intersection would be before it
	inter.t = DBL_MAX;

	// Create a temporary intersection object to store the intersection data for the sides.
	intersection temp = inter;

	// Create a variable to store if any intersection has been found.
	bool anyIntersection = false;

	// Run the Intersect method for every side with different intersection objects
	// compare the t's of the intersection objects and set the cube's intersection object to the earliest t
	// return false if all intersects return false.
	for (int i = 0; i < 6; i++) {
		if (side[i].Intersect(R, temp)) {
			anyIntersection = true;

			// temp.t is already insured to be greater than 0 by the Intersect method
			// Intersect would return false if t were less than 0
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
		// if no intersection with any side
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
