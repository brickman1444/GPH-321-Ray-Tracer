// For the Foruth example, we add a Label control to the 
// bottom of the window which displays the position of the
// Sphere

#include "win.h"
#include "point.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "polygon.h"
#include "scene.h"
#include "material.h"
#include "light.h"
#include "plane.h"
#include "Cube.h"

class Application : public EventListener
{
public:

	Application();

private:

	AppWindow	Win;

	plane floor, ceiling, wall_1, wall_2, wall_3, wall_4;
	cube box;
	phongMaterial m;
	reflectivePhong ref;
	checkerboardPhong checks;
	glass glassMat;
	ambientLight amb;
	pointLight light_1;
	scene s;

	// Event Handlers
	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);

public:

};

Application::Application()
{ 
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Ray Tracing");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	floor = plane(point(0,0,0), vector(0,0,1));
	floor.SetMaterial(checks);
	s.shapes.Append(&floor);

	box = cube(point(2,3,0), vector(0,0,1), vector(1,1,0), 2); 
	box.SetMaterial(m);
	box.SetColor(rgb::red);
	s.shapes.Append(&box);

	ceiling = plane(point(0,0,10), vector(0,0,1));
	ceiling.SetMaterial(checks);
	ceiling.SetCanShadow(false);
	//s.shapes.Append(&ceiling);

	wall_1 = plane(point(0,0,0), vector(1,0,0));
	wall_1.SetShininess(20);
	wall_1.SetSpecularColor(.2, .2, .2);
	wall_1.SetColor(rgb::lightGray);
	wall_1.SetMaterial(m);
	s.shapes.Append(&wall_1);

	wall_2 = plane(point(0,0,0), vector(0,1,0));
	wall_2.SetShininess(20);
	wall_2.SetSpecularColor(.2, .2, .2);
	wall_2.SetColor(rgb::lightGray);
	wall_2.SetMaterial(m);
	s.shapes.Append(&wall_2);

	wall_3 = plane(point(10,0,0), vector(1,0,0));
	wall_3.SetShininess(20);
	wall_3.SetSpecularColor(.2, .2, .2);
	wall_3.SetMaterial(m);
	wall_3.SetCanShadow(false);
	//s.shapes.Append(&wall_3);

	wall_4 = plane(point(0,10,0), vector(0,1,0));
	wall_4.SetShininess(20);
	wall_4.SetSpecularColor(.2, .2, .2);
	wall_4.SetMaterial(m);
	wall_4.SetCanShadow(false);
	//s.shapes.Append(&wall_4);

	/*light_1.SetDiffuseColor(2.0 * rgb::offWhite);
	light_1.SetSpecularColor(3.0 * rgb::white);
	light_1.SetPosition(point(10, 5, 5));
	light_1.SetAttenuations(1, 0, .015);
	light_1.SetAngleLimits(360, 360);
	light_1.SetShadowColor(rgb(.5, .5, .5));*/
	//light_1 = distantLight(vector(0,0,-1), rgb::white, rgb::white, rgb::lightGray, true); 
	light_1 = pointLight(point(7,7,7), rgb(.9,.9,.9), rgb::white, rgb(.1,.1,.1), true);
	s.lights.Append(&light_1);

	s.camera = point(10, 5, 5);
	s.target = point(2, 2, 0);
	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.5;
	s.windowH = 1.5;
	s.backgroundColor = rgb::green;

	//s.DrawScene("test.bmp");
	s.DrawSceneAntialias(10,10,"test10x10.bmp");
}

void Application::UpdateWindow(intRect UpdateRect)
{
	DrawScene();
}

void Application::DrawScene(void)
{
	s.CopySceneToWindow();
}

void main()
{
	Application app;

	HandleEvents();

	EventLoop();
}

