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
	cube box1, box2, box3;
	sphere globe1, globe2;
	phongMaterial m;
	reflectivePhong ref;
	reflectiveCheckerboardPhong shinyChecks;
	checkerboardPhong checks;
	rainbowPhong rainbow;
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

	ref.SetReflectiveIndex(.1);
	ref.SetScene(s);

	shinyChecks.SetReflectiveIndex(.2);
	shinyChecks.SetScene(s);

	floor = plane(point(0,0,0), vector(0,0,1));
	floor.SetMaterial(shinyChecks);
	floor.SetColor(rgb::lightGray);
	s.shapes.Append(&floor);

	box1.SetSideLength(2);
	box1.SetTranslation(2,3,0);
	box1.SetZRotation(45);
	box1.SetYRotation(-45);
	box1.SetXRotation(-45);
	box1.SetMaterial(m);
	box1.SetColor(rgb::red);
	box1.CalculateSides();
	s.shapes.Append(&box1);
	
	box2.SetMaterial(rainbow);
	box2.SetColor(rgb::white);
	box2.SetTranslation(5,1,0);
	box2.SetScale(1,2,.5);
	box2.CalculateSides();
	s.shapes.Append(&box2);

	box3.SetTranslation(4,4.75,0);
	box3.SetSideLength(.5);
	box3.SetMaterial(m);
	box3.SetColor(rgb::blue);
	box3.CalculateSides();
	s.shapes.Append(&box3);

	globe1.SetTranslation(3.5,3,2.75);
	globe1.SetRadius(.75);
	globe1.SetMaterial(rainbow);
	globe1.SetShininess(10);
	globe1.SetColor(rgb::orange);
	s.shapes.Append(&globe1);

	globe2.SetTranslation(5,4,.5);
	globe2.SetRadius(.5);
	globe2.SetMaterial(m);
	globe2.SetShininess(10);
	globe2.SetColor(rgb::cyan);
	s.shapes.Append(&globe2);

	ceiling = plane(point(0,0,10), vector(0,0,1));
	ceiling.SetMaterial(checks);
	ceiling.SetCanShadow(false);
	//s.shapes.Append(&ceiling);

	wall_1 = plane(point(0,0,0), vector(1,0,0));
	wall_1.SetShininess(20);
	wall_1.SetSpecularColor(.2, .2, .2);
	wall_1.SetColor(rgb::white);
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

	//s.camera = point(4, 0, 2);
	//s.target = point(5, 1, 1);

	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.5;
	s.windowH = 1.5;

	//s.backgroundColor = rgb::green;

	s.DrawScene();
	//s.DrawScene("test.bmp");
	//s.DrawSceneAntialias(1,1,"test10x10.bmp");
	//s.DrawSceneMonteCarlo(100,"test2.bmp");
	//s.DrawSceneMonteCarloAdvanced(4, 32, .01, "test1.bmp");
	//s.DrawSceneEdgeDetect( .01, "test1.bmp");
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

