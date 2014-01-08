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
	cube box1, box2, box3;
	sphere globe1, globe2;
	octahedron octo;
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
	floor.SetMaterial(m);
	floor.SetColor(rgb::lightGray);
	s.shapes.Append(&floor);

	light_1 = pointLight(point(7,7,7), rgb(.9,.9,.9), rgb::white, rgb(.1,.1,.1), true);
	s.lights.Append(&light_1);

	s.camera = point(5, 6, 5);
	s.target = point(0, 0, 0);

	octo.SetMaterial(rainbow);
	octo.SetRadius(2);
	octo.SetColor(rgb::gray);
	s.shapes.Append(&octo);

	globe1.SetMaterial(m);
	//s.shapes.Append(&globe1);

	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.5;
	s.windowH = 1.5;

	//s.backgroundColor = rgb::green;

	s.DrawScene();
	//s.DrawScene("test.bmp");
	//s.DrawSceneAntialias(4,4,"test10x10.bmp");
	//s.DrawSceneMonteCarlo(100,"test2.bmp");
	//s.DrawSceneMonteCarloAdvanced(4, 16, .01, "test1.bmp");
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

