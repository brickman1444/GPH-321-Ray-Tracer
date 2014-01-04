#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "color.h"
#include "vector.h"
#include "BitmapFile.h"
#include "Light.h"

struct intersection;
struct lightOutput;
class shape;

////////////////////////////////////////////////////////////////////
// Utility functions
////////////////////////////////////////////////////////////////////

// For both of these, the vector v is assumed to be "outgoing"
// from the surface.  Note that this is different from the Renderman
// and GLSL standards!
inline vector faceforward(const vector &n, const vector &v)
{
	if (Dot(n, v) < 0)
		return -n;
	else
		return n;
}

inline vector reflect(const vector &n, const vector &v)
{
	return -v + 2 * Dot(n, v) * n;
}

inline vector reflectIncoming(const vector &n, const vector &I)
{
	return I - 2 * Dot(n, I) * n;
}

inline double smoothstep(double a, double b, double val)
{
	double percent = (val - a) / (b - a);
	
	if (percent < 0.0)
		return 0.0;
	else if (percent > 1.0)
		return 1.0;
	else
		return (3 * percent - 2 * percent * percent) * percent;
}

////////////////////////////////////////////////////////////////////
// Material Classes
////////////////////////////////////////////////////////////////////

class material
{
public:

	// This function returns a rgb reflected from a surface
	// The default implementation of this method returns black ... no reflectance
	// A derived class should over-ride this method.
	virtual void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
								const lightOutput &lightVal, const intersection &inter) = 0;
	virtual rgb ReflectLights(intersection &inter, LinkedList<light *> &lights, LinkedList<shape *> &shapes);
};

class constantMaterial : public material
{
public:

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
	virtual rgb ReflectLights(intersection &inter, LinkedList<light *> &lights, LinkedList<shape *> &shapes);
};

class fakeMaterial : public constantMaterial
{
public:

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
};

class lambertianMaterial : public material
{
public:

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
};

class phongMaterial : public material
{
public:

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
};

class blinnMaterial : public material
{
public:

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
protected:

};

class scene;

/////////////////////////////////////////////////////////////////////////////////////////////////
// A simple x-aligned stripe pattern derived from phong
/////////////////////////////////////////////////////////////////////////////////////////////////

class stripedPhong : public phongMaterial
{
public:

	stripedPhong(double sw = 1, const rgb &Color1 = rgb::black, const rgb &Color2 = rgb::white)
	{ stripeWidth = sw; color1 = Color1; color2 = Color2; }

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);

protected:

	double stripeWidth;
	rgb color1, color2;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// A simple checkerboard pattern material derived from phong
/////////////////////////////////////////////////////////////////////////////////////////////////

class checkerboardPhong : public phongMaterial
{
public:

	checkerboardPhong(double cL = 1, double cW = 1, double cH = 1, 
					  const rgb &CDark = rgb::black, const rgb &CLight = rgb::white)
	{ checkL = cL; checkW = cW; checkH = cH; cDark = CDark; cLight = CLight; }

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);

protected:

	double checkL, checkW, checkH;
	rgb cDark, cLight;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
// A simple texture map that uses the "tex" member of the intersection to texture
// an object with an image
//////////////////////////////////////////////////////////////////////////////////////////////////

class texturedPhong : public phongMaterial
{
public:
	 
	texturedPhong(char *filename);

	void LoadTexture(char *filename);
	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);

protected:

	BitmapFile texture;
};

/////////////////////////////////////////////////////////////////////////////////////////////////
// Reflective materials
/////////////////////////////////////////////////////////////////////////////////////////////////

void MaterialSetMaxReflectionLevel(int k);

class reflectivePhong : public phongMaterial
{
public:

	reflectivePhong() { world = NULL; reflectiveIndex = 0; }
	reflectivePhong(scene &S, double RI = 1.0);

	void SetScene(scene &S) { world = &S; }
	scene &GetScene(void) { return *world; }
	void SetReflectiveIndex(double ref) { reflectiveIndex = ref; }

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
protected:
	
	scene *world;
	double reflectiveIndex;
};

class glass : public material
{
public:

	glass() { kR = 1.0; kT = 1.0; materialN = 1.0; ambientN = 1.0; transmitColor = rgb::white; world = NULL; 
			  kA = 0.2; kD = 0.0; kS = 0.5; }
	glass(scene &S);

	void SetReflectiveAmount(double val) { kR = val; }
	void SetTransmissiveAmount(double val) { kT = val; }
	void SetAmbientAmount(double val) { kA = val; }
	void SetDiffuseAmount(double val) { kD = val; }
	void SetSpecularAmount(double val) { kS = val; }
	void SetMaterialIndex(double val) { materialN = val; }
	void SetAmbientIndex(double val) { ambientN = val; }
	void SetTransmissiveColor(rgb val) { transmitColor = val; }
	void SetScene(scene &S) { world = &S; }
	double GetKr(void) { return kR; }
	double GetKt(void) { return kT; }
	double GetMaterialRefI(void) { return materialN; }
	double GetAmbientRefI(void) { return ambientN; }
	rgb &GetTransmissiveColor(void) { return transmitColor; }
	scene &GetScene(void) { return *world; }

	void ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
						const lightOutput &lightVal, const intersection &inter);
protected:
	
	scene *world;
	double kR, kT;
	double kA, kD, kS; 
	double materialN;
	double ambientN; // This should be replaced with a system that tracks the ray
	rgb transmitColor;
};

#endif
