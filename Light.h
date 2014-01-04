#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "color.h"
#include "point.h"
#include "vector.h"

struct intersection;

struct lightOutput
{
public:

	rgb a, d, s;
	vector L;
};

class light
{
public:

	light(const rgb &dc = rgb::white, const rgb &sc = rgb::white, const rgb &ac = rgb::black) 
	{ aC = ac; dC = dc; sC = sc; canShadow = true; hasPosition = false; shadowColor = rgb(.2, .2, .2); }

	void SetAmbientColor(const rgb &c) { aC = c; }
	void SetDiffuseColor(const rgb &c) { dC = c; }
	void SetSpecularColor(const rgb &c) { sC = c; }

	rgb GetAmbientColor(void) { return aC; }
	rgb GetDiffuseColor(void) { return dC; }
	rgb GetSpecularColor(void) { return sC; }

	bool CanShadow(void) { return canShadow; }
	bool CalculateShadow(const intersection &inter, lightOutput &output);
	void SetShadowColor(const rgb &newColor) { shadowColor = newColor; }

	// A derived class should override this method.  It should fill the 
	// ambient, diffuse and specular light quantities respectively
	// and then return a bool indicating whether p receives light from 
	// this light.  L should be the unit vector pointing to the light.
	virtual bool Illuminance(const intersection &inter, lightOutput &output) 
	{ return false; }

	bool HasPosition(void) { return hasPosition; }
	point &GetPosition(void) { return pos; }

protected:

	rgb aC, dC, sC;  // ambient, diffuse and specular light colors

	bool canShadow;
	bool hasPosition;
	rgb shadowColor;
	point pos;
};

class ambientLight : public light
{
public:

	ambientLight(const rgb &c = rgb::white) { SetColor(c); canShadow = false; }
	void SetColor(const rgb &c) { aC = c; }

	bool Illuminance(const intersection &inter, lightOutput &output);
};

class distantLight : public light
{
public:

	distantLight(const vector &d = vector(0.0, 0.0, 1.0), const rgb &dc = rgb::white, 
				 const rgb &sc = rgb::white, const rgb &ac = rgb::black, bool shad = true);

	void SetDirection(const vector &d) { dir = d; dir.Normalize(); pos = point(0, 0, 0) + dir * 1e16; }
	vector GetDirection(void) { return dir; }

	bool Illuminance(const intersection &inter, lightOutput &output);

protected:

	// dir should be the direction to the light from any point in space
	vector dir;
};

class pointLight : public light
{
public:

	pointLight(const point &p = point(1, 1, 1), const rgb &dc = rgb::white,
		const rgb &sc = rgb::white, const rgb &ac = rgb::black, double cAtten = 1.0, 
		double lAtten = 0.0, double qAtten = 0.0);

	void SetPosition(const point &p) { pos = p; }
	void SetAttenuations(double C, double L, double Q) { attenC = C; attenL = L; attenQ = Q; }

	double GetAttenC(void) { return attenC; }
	double GetAttenL(void) { return attenL; }
	double GetAttenQ(void) { return attenQ; }

	bool Illuminance(const intersection &inter, lightOutput &output);

protected:

	double attenC, attenL, attenQ;
};

class spotLight : public light
{
public:

	spotLight(const point &p = point(1, 1, 1), const rgb &sc = rgb::white, 
		const point &Target = point(0, 0, 0), double InnerThet = 30, double OuterThet = 40, 
		double cAtten = 1.0, double lAtten = 0.0, double qAtten = 0.0);

	void SetPosition(const point &p) { pos = p; }
	void SetTarget(const point &t) { target = t; }
	void SetAttenuations(double C, double L, double Q) { attenC = C; attenL = L; attenQ = Q; }
	void SetAngleLimits(double inner, double outer) { innerThet = inner; outerThet = outer; }

	double GetAttenC(void) { return attenC; }
	double GetAttenL(void) { return attenL; }
	double GetAttenQ(void) { return attenQ; }

	bool Illuminance(const intersection &inter, lightOutput &output);

protected:

	double attenC, attenL, attenQ;
	point target;
	double innerThet, outerThet;
};

#endif