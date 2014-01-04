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

class Application : public EventListener
{
public:

	Application();

private:

	AppWindow	Win;

	sphere globe;
	polygon floor, ceiling, wall_1, wall_2, wall_3, wall_4;
	phongMaterial m;
	reflectivePhong ref;
	checkerboardPhong checks;
	ambientLight amb;
	spotLight light_1;
	scene s;

	// Event Handlers
	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);

public:

};

Application::Application()
{ 
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Ray Tracing - Testing Procedural Texture");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	ref.SetReflectiveIndex(.5);
	ref.SetScene(s);

	globe.Transform(matrix::Translation(3, 2.5, 2));
	globe.Transform(matrix::Scale(1.5, 1.5, 1.5));
	globe.SetColor(0.1, 0.2, .5);
	globe.SetSpecularColor(.7, .7, 1);
	globe.SetShininess(40);
	globe.SetMaterial(ref);
	globe.SetCanShadow(true);
	s.shapes.Append(&globe); 

	floor.AddPoint(point(0, 0, 0));
	floor.AddPoint(point(10, 0, 0));
	floor.AddPoint(point(10, 10, 0));
	floor.AddPoint(point(0, 10, 0));
	floor.CalculateNormal();
	floor.SetMaterial(checks);
	s.shapes.Append(&floor);

	ceiling.AddPoint(point(0, 0, 9.8));
	ceiling.AddPoint(point(10, 0, 10));
	ceiling.AddPoint(point(10, 10, 10));
	ceiling.AddPoint(point(0, 10, 10));
	ceiling.CalculateNormal();
	ceiling.SetMaterial(checks);
	ceiling.SetCanShadow(false);
	s.shapes.Append(&ceiling);

	wall_1.AddPoint(point(0, 0, 0));
	wall_1.AddPoint(point(0, 10, 0));
	wall_1.AddPoint(point(0, 10, 10));
	wall_1.AddPoint(point(0, 0, 10));
	wall_1.CalculateNormal();
	wall_1.SetShininess(20);
	wall_1.SetSpecularColor(.4, .4, .4);
	wall_1.SetMaterial(m);
	s.shapes.Append(&wall_1);

	wall_2.AddPoint(point(0, 0, 0));
	wall_2.AddPoint(point(0, 0, 10));
	wall_2.AddPoint(point(10, 0, 10));
	wall_2.AddPoint(point(10, 0, 0));
	wall_2.CalculateNormal();
	wall_2.SetShininess(20);
	wall_2.SetSpecularColor(.4, .4, .4);
	wall_2.SetMaterial(m);
	s.shapes.Append(&wall_2);

	wall_3.AddPoint(point(10, 0, 0));
	wall_3.AddPoint(point(10, 10, 0));
	wall_3.AddPoint(point(10, 10, 10));
	wall_3.AddPoint(point(10, 0, 10));
	wall_3.CalculateNormal();
	wall_3.SetShininess(20);
	wall_3.SetSpecularColor(.4, .4, .4);
	wall_3.SetMaterial(m);
	wall_3.SetCanShadow(false);
	s.shapes.Append(&wall_3);

	wall_4.AddPoint(point(0, 10, 0));
	wall_4.AddPoint(point(0, 10, 10));
	wall_4.AddPoint(point(10, 10, 10));
	wall_4.AddPoint(point(10, 10, 0));
	wall_4.CalculateNormal();
	wall_4.SetShininess(20);
	wall_4.SetSpecularColor(.4, .4, .4);
	wall_4.SetMaterial(m);
	wall_4.SetCanShadow(false);
	s.shapes.Append(&wall_4);

	light_1.SetDiffuseColor(2.0 * rgb::offWhite);
	light_1.SetSpecularColor(3.0 * rgb::white);
	light_1.SetPosition(point(10, 5, 5));
	light_1.SetAttenuations(1, 0, .015);
	light_1.SetAngleLimits(360, 360);
	light_1.SetShadowColor(rgb(.5, .5, .5));
	s.lights.Append(&light_1);

	s.camera = point(8.0, 2.0, 5);
	s.target = point(3, 2.5, 2);
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

	HandleEvents();

	EventLoop();
}

