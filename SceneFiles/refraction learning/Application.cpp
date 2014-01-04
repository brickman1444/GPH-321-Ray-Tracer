
#include "win.h"
#include "sphere.h"
#include "scene.h"

class Application : public EventListener
{
public:

	Application();

private:

	AppWindow	Win;

	sphere				globe;
	polygon				wall_1, wall_2;
	phongMaterial		m;
	distantLight		light1;
	glass				glassMat;
	scene				s;

	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);
};

Application::Application()
{ 
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Ray Tracing - Phong Material");

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

	globe.SetCenter(point(0.0, 0.0, 0.0));
	globe.SetRadius(1.0);
	globe.SetColor(.99, .99, .99);
	globe.SetSpecularColor(.7, .7, 1);
	globe.SetShininess(40);
	globe.SetMaterial(glassMat);
	globe.SetCanShadow(true);
	s.shapes.Append(&globe); 

	wall_1.AddPoint(point(-4, -10, -10));
	wall_1.AddPoint(point(-4, 10, -10));
	wall_1.AddPoint(point(-4, 10, 10));
	wall_1.AddPoint(point(-4, -10, 10));
	wall_1.CalculateNormal();
	wall_1.SetShininess(20);
	wall_1.SetColor(1, 0, 0);
	wall_1.SetSpecularColor(1, 0, 0);
	wall_1.SetMaterial(m);
	s.shapes.Append(&wall_1);

	wall_2.AddPoint(point(-10, -10, -1));
	wall_2.AddPoint(point(-10, 10, -1));
	wall_2.AddPoint(point(10, 10, -1));
	wall_2.AddPoint(point(10, -10, -1));
	wall_2.CalculateNormal();
	wall_2.SetShininess(20);
	wall_2.SetColor(0, 1, 0);
	wall_2.SetSpecularColor(0, 1, 0);
	wall_2.SetMaterial(m);
	s.shapes.Append(&wall_2);

	light1.SetDirection(vector(8, 3, 5));
	light1.SetAmbientColor(.7 * rgb::darkGray);
	light1.SetDiffuseColor(.75 * rgb::offWhite);
	light1.SetSpecularColor(rgb::white);

	s.lights.Append(&light1);

	s.camera = point(4.0, 0.0, 0.0);
	s.target = point(0.0, 0.0, 0.0);
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

