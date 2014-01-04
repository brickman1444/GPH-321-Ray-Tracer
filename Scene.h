#ifndef __SCENE_H__
#define __SCENE_H__

#include "point.h"
#include "vector.h"
#include "sphere.h"
#include "polygon.h"
#include "OpenGLWin.h"
#include "Bitmap.h"
#include "LinkedList.h"

class scene
{
public:
	
	scene() { bit = NULL; backgroundColor = rgb::black; }
	~scene() { if (bit != NULL) delete bit; }

	void DrawScene(char *outputFileName = NULL);
	void DrawSceneAntialias(int xSamples, int ySamples, char *outputFileName = NULL);
	void DrawSceneMonteCarlo(int samples, char *outputFileName = NULL);
	void CopySceneToWindow(void);

	// This casts a ray and returns an intersection, but no color is applied
	bool CastRay(const ray &r, intersection &inter);

	// This method performs a full ray trace including material and lighting 
	// at the resulting intersection
	rgb	 TraceRay(const ray &r);
	
	// Scene setup data
	point camera;
	point target;
	vector up;
	double windowW, windowH;
	double windowD;
	rgb backgroundColor;
	AppWindow *win;
	Bitmap *bit;

	LinkedList<shape *> shapes;
	LinkedList<light *> lights;
};

#endif