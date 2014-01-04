
#include "win.h"
#include "Sphere.h"
#include "Polygon.h"
#include "scene.h"

class Application : public EventListener
{
public:

	Application();

private:

	AppWindow	Win;

	sphere				globe;
	polygon				floor, wall1, wall2;
	phongMaterial		globeMat;
	lambertianMaterial	floorWallMat;
	pointLight			light1;
	scene				s;

	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);
};

Application::Application()
{ 
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Ray Tracing - Scene with Phong Material");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	globe.SetCenter(point(3, 2, 1));
	globe.SetRadius(1.0);
	globe.SetColor(rgb::violet);	
	globe.SetSpecularColor(rgb::white);
	globe.SetShininess(50);
	globe.SetMaterial(globeMat);
	s.shapes.Append(&globe); 

	floor.AddPoint(point(0, 0, 0));
	floor.AddPoint(point(10, 0, 0));
	floor.AddPoint(point(10, 10, 0));
	floor.AddPoint(point(0, 10, 0));
	floor.CalculateNormal();
	floor.SetColor(rgb::brown);
	floor.SetSpecularColor(rgb::black);
	floor.SetMaterial(floorWallMat);
	s.shapes.Append(&floor);

	wall1.AddPoint(point(0, 0, 0));
	wall1.AddPoint(point(0, 10, 0));
	wall1.AddPoint(point(0, 10, 10));
	wall1.AddPoint(point(0, 0, 10));
	wall1.CalculateNormal();
	wall1.SetColor(rgb(.95, .90, .80));
	wall1.SetSpecularColor(rgb::black);
	wall1.SetMaterial(floorWallMat);
	s.shapes.Append(&wall1);

	wall2.AddPoint(point(0, 0, 0));
	wall2.AddPoint(point(0, 0, 10));
	wall2.AddPoint(point(10, 0, 10));
	wall2.AddPoint(point(10, 0, 0));
	wall2.CalculateNormal();
	wall2.SetColor(rgb(.95, .90, .80));
	wall2.SetSpecularColor(rgb::black);
	wall2.SetMaterial(floorWallMat);
	s.shapes.Append(&wall2);

	light1.SetPosition(point(8, 15, 13));
	light1.SetAmbientColor(.7 * rgb::darkGray);
	light1.SetDiffuseColor(rgb(1.0, .9, .7));
	light1.SetSpecularColor(rgb::white);

	s.lights.Append(&light1);

	s.camera = point(10, 4.8, 5.2);
	s.target = point(0.0, .5, -.25);
	s.up = vector(0, 0, 1);
	s.win = &Win;
	s.windowD = 2;
	s.windowW = 1.5;
	s.windowH = 1.5;

	s.DrawScene();
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

	EventLoop();
}

