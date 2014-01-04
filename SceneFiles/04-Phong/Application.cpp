
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
	phongMaterial		globeMat;
	distantLight		light1;
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

	globe.SetCenter(point(0.0, 0.0, 0.0));
	globe.SetRadius(1.0);
	globe.SetColor(rgb::chartreuse);	
	globe.SetSpecularColor(rgb::white);
	globe.SetShininess(350);
	globe.SetMaterial(globeMat);

	s.shapes.Append(&globe); 

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

