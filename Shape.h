#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "point.h"
#include "color.h"
#include "vector.h"
#include "ray.h"
#include "LinkedList.h"
#include "matrix.h"
#include "material.h"
#include "light.h"

struct intersection
{
	double	t;			// Value of t on the ray at which the intersection occurred
	point	p;			// Position of the intersection
	vector	n;			// Normal of the surface at the intersection point
	vector  v;			// Outgoing view vector to intersection (i.e. this is the negative of the ray vector!)
	point   tex;		// Coordinates for applying a texture
	rgb		diff;		// Surface diffuse = ambient color
	rgb		spec;		// Surface specular
	double	shiny;		// Surface shininess
	class shape *s;		// The shape on which the intersection occurred (for materials)
	class scene *w;		// The world in which the intersection occurred (for shadows and reflections)
};

class shape
{
public:

	shape();

	void SetColor(double R, double G, double B) { surfaceColor = rgb(R, G, B); }
	void SetColor(const rgb &c) { surfaceColor = c; }
	void SetSpecularColor(double R, double G, double B) { specularColor = rgb(R, G, B); }
	void SetSpecularColor(const rgb &c) { specularColor = c; }
	void SetShininess(double s) { shininess = s; }

	// These are returned by reference to avoid a copy.  The last one
	// is by reference for consistency, but little is gained.
	const rgb &Color(void) { return surfaceColor; }
	const rgb &Specular(void) { return specularColor; }
	const double &Shininess(void) { return shininess; }

	// Intersection method, applies transform to ray and calls "Intersect" below
	bool CalculateIntersection(const ray &R, intersection &I);

	// Question:  Should these three be private?
	virtual bool Intersect(const ray &R, intersection &I) { return false; }
	vector TransformNormal(const vector &n);
	ray TransformRay(const ray &r);

	// Methods for setting and applying materials
	virtual void SetMaterial(material &m) { objectMat = &m; }
	virtual void SetMaterial(material *m) { objectMat = m; }
	virtual rgb ApplyMaterial(intersection &L, LinkedList<light *> &lights, LinkedList<shape *> &shapes);

	// Methods for shadow properties
	void SetCanShadow(bool flag) { canShadow = flag; }
	bool CanShadow(void) { return canShadow; }
	void SetCanBeShadowed(bool flag) { canBeShadowed = flag; }
	bool CanBeShadowed(void) { return canBeShadowed; }

	// Methods for setting transforms
	void SetTranslation(double x, double y, double z) { SetTranslation(point(x, y, z)); }
	void SetTranslation(const point &p);
	void SetXRotation(double angle);
	void SetYRotation(double angle);
	void SetZRotation(double angle);
	void SetRotation(double xRot, double yRot, double zRot);
	void SetScale(double x, double y, double z) { SetScale(vector(x, y, z)); }
	void SetScale(const vector &s);

	// Method for transforming. Is cumulative with all transformations so far.
	//void Transform(matrix m) {totalTransform = m * totalTransform; RecalculateMatrices(); }

	// Resets total transformation to the identity matrix.
	void ResetTransformations(void) {totalTransform = matrix::Identity(4); }

protected:

	bool canShadow;
	bool canBeShadowed;

	rgb surfaceColor;	
	rgb specularColor;
	double shininess;

	material *objectMat;

	point translation;
	double rotX, rotY, rotZ;
	vector scale;

	matrix totalTransform;
	matrix inverseTransform;
	matrix inverseTranspose;

	void RecalculateMatrices();

	static constantMaterial defaultMaterial;
};

#endif