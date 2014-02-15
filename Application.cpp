// For the Foruth example, we add a Label control to the 
// bottom of the window which displays the position of the
// Sphere

#include "win.h"
#include "point.h"
#include "matrix.h"
#include "ray.h"
#include "sphere.h"
#include "polygon.h"
#include "octahedron.h"
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
	polygon poly;
	cube box1, box2, box3;
	sphere globe1, globe2;
	octahedron octo1, octo2, octo3;

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
	Win.Setup(0, 0, 400, 400, false);
	Win.SetText("Ray Tracer");
	Win.AddMainMenu();
	Win.AddFileMenu();
	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	ref.SetReflectiveIndex(.3);
	ref.SetScene(s);

	shinyChecks.SetReflectiveIndex(.25);
	shinyChecks.SetScene(s);

	floor = plane(point(0,0,0), vector(0,0,1));
	floor.SetMaterial(shinyChecks);
	floor.SetColor(rgb::lightGray);
	s.shapes.Append(&floor);

	light_1 = pointLight(point(7,2,7), rgb(.9,.9,.9), rgb::white, rgb(.1,.1,.1), true);
	s.lights.Append(&light_1);

	s.camera = point(5, 4, 3);
	s.target = point(0, 0, 0);

	octo2.SetMaterial(m);
	octo2.SetTranslation(0,0,.5);
	octo2.SetColor(rgb::red);
	octo2.SetScale(.5,.5,.5);
	octo2.SetRotation(0,0,0);
	octo2.SetTranslation(2.25,.5,.5);
	s.shapes.Append(&octo2);

	rainbow = rainbowPhong(2,2,2);
	box1.SetMaterial(rainbow);
	box1.SetColor(rgb::white);
	box1.SetScale(1,1,1.5);
	box1.SetRotation(0,0,5);
	box1.SetTranslation(.25,.1,0);
	box1.CalculateSides();
	s.shapes.Append(&box1);

	globe1.SetMaterial(ref);
	globe1.SetColor(rgb::yellow);
	globe1.SetTranslation(1,2,.5);
	globe1.SetScale(.5,.5,.5);
	globe1.SetRotation(0,10,-150);
	s.shapes.Append(&globe1);

	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.4;
	s.windowH = 1.4;

	//s.DrawScene();
	s.DrawScene("renderDefault.bmp");
	s.DrawSceneAntialias(4,4,"render4x4Array.bmp");
	s.DrawSceneMonteCarlo(16,"renderMonteCarlo16.bmp");
	s.DrawSceneMonteCarloAdvanced(4, 16, .01, "renderMonteCarloAdvanced4-16.bmp");
	s.DrawSceneEdgeDetect( .01, "renderEdgeDetection.bmp");
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

