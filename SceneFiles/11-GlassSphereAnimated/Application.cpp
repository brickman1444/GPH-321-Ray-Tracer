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
	polygon floor_1, ceiling_1, wall_1, wall_2, wall_3, wall_4;
	phongMaterial m;
	reflectivePhong ref;
	checkerboardPhong checks;
	glass glassMat;
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
	Win.SetText("Ray Tracing");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	glassMat.SetScene(s);
	glassMat.SetReflectiveAmount(1.0);
	glassMat.SetTransmissiveAmount(1.0);
	glassMat.SetMaterialIndex(1.5);
	glassMat.SetAmbientAmount(0.0);
	glassMat.SetSpecularAmount(0.8);
	glassMat.SetTransmissiveColor(rgb(.7, .7, 1.0));

	globe.SetColor(.99, .99, .99);
	globe.Transform(matrix::Translation(3, 2.5, 2));
	globe.Transform(matrix::Scale(1.5, 1.5, 1.5));
	globe.SetColor(0, 0, 0);
	globe.SetSpecularColor(.7, .7, 1);
	globe.SetShininess(40);
	globe.SetMaterial(glassMat);
	globe.SetCanShadow(true);
	s.shapes.Append(&globe); 

	floor_1.AddPoint(point(0, 0, 0));
	floor_1.AddPoint(point(10, 0, 0));
	floor_1.AddPoint(point(10, 10, 0));
	floor_1.AddPoint(point(0, 10, 0));
	floor_1.CalculateNormal();
	floor_1.SetMaterial(checks);
	s.shapes.Append(&floor_1);

	ceiling_1.AddPoint(point(0, 0, 9.8));
	ceiling_1.AddPoint(point(10, 0, 10));
	ceiling_1.AddPoint(point(10, 10, 10));
	ceiling_1.AddPoint(point(0, 10, 10));
	ceiling_1.CalculateNormal();
	ceiling_1.SetMaterial(checks);
	ceiling_1.SetCanShadow(false);
	s.shapes.Append(&ceiling_1);

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

	double theta = .2;
	int i = 0;
	for (double theta = .2; theta < PI/2.0 - .2; theta += .01, i++)
	{
		char filename[50];

		double dist = 8;
		s.camera = point(dist * cos(theta), dist * sin(theta), 5);
		cout << s.camera << endl;
		s.target = point(3, 2.5, 2);
		s.up = vector(0, 0, 1);
		s.win = &Win;
		s.windowD = 2;
		s.windowW = 1.5;
		s.windowH = 1.5;

		s.DrawScene();
		sprintf(filename, "Animation1\\Picture%03d.bmp", i);
		s.DrawScene(filename);
	}
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

