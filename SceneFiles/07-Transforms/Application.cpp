
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

	sphere				center, petals[200];
	sphere				stem, leaves[2];
	polygon				floor, wall1, wall2;
	phongMaterial		flowerMat;
	lambertianMaterial	floorWallMat;
	pointLight			light1;
	scene				s;

	void UpdateWindow(intRect UpdateRect);
	void DrawScene(void);
};

Application::Application()
{ 
	Win.Setup(0, 0, 500, 500, false);
	Win.SetText("Ray Tracing - Testing Transforms");

	Win.AddMainMenu();
	Win.AddFileMenu();

	Win.SetPaintEvent(this, &Application::UpdateWindow);
	Win.Show();

	center.SetColor(1, 1, 0);
	center.Transform(matrix::Translation(3, 2, 3));
	center.Transform(matrix::Scale(.2, .6 ,.6));
	center.SetSpecularColor(rgb::white);
	center.SetShininess(20.0);
	center.SetMaterial(flowerMat);
	s.shapes.Append(&center); 

	for (int i = 0; i < 50; i++)
	{
		double angle = 7.2 * i;
		if (angle < 180)
			petals[i].SetColor(angle / 180.0, 1.0 - angle / 180.0, .1);
		else
			petals[i].SetColor(1.0 - (angle - 180) / 180.0, (angle - 180) / 180.0, .1);
		petals[i].Transform(matrix::Translation(3, 2, 3));
		petals[i].Transform(matrix::RotationY(90));
		petals[i].Transform(matrix::RotationZ(angle));
		petals[i].Transform(matrix::Scale(.5, .1, .1));
		petals[i].Transform(matrix::Translation(2, 0, 0));
		petals[i].SetSpecularColor(rgb::white);
		petals[i].SetShininess(20.0);
		petals[i].SetMaterial(flowerMat);
		s.shapes.Append(&petals[i]);
	}

	stem.SetColor(.2, 1, .2);
	stem.Transform(matrix::Translation(3, 2, 1));
	stem.Transform(matrix::Scale(.1, .1, 2));
	stem.SetSpecularColor(rgb::white);
	stem.SetShininess(20.0);
	stem.SetMaterial(flowerMat);
	s.shapes.Append(&stem);

	leaves[0].SetColor(.2, 1, .2);
	leaves[0].Transform(matrix::Translation(3, 2, 1));
	leaves[0].Transform(matrix::RotationX(20));
	leaves[0].Transform(matrix::Translation(0, 1, 0));
	leaves[0].Transform(matrix::Scale(.01, 1, .3));
	leaves[0].SetSpecularColor(rgb::white);
	leaves[0].SetShininess(20.0);
	leaves[0].SetMaterial(flowerMat);
	s.shapes.Append(&leaves[0]);

	leaves[1].SetColor(.2, 1, .2);
	leaves[1].Transform(matrix::Translation(3, 2, .5));
	leaves[1].Transform(matrix::RotationX(160));
	leaves[1].Transform(matrix::Translation(0, 1, 0));
	leaves[1].Transform(matrix::Scale(.01, 1, .3));
	leaves[1].SetSpecularColor(rgb::white);
	leaves[1].SetShininess(20.0);
	leaves[1].SetMaterial(flowerMat);
	s.shapes.Append(&leaves[1]);

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

	light1.SetPosition(point(15, 9, 10));
	light1.SetAmbientColor(rgb(.2, .2, .2));
	light1.SetDiffuseColor(2.0 * rgb(1.0, .9, .7));
	light1.SetAttenuations(1.0, 0.0, .01);
	light1.SetSpecularColor(rgb::white);
	s.lights.Append(&light1);

	s.camera = point(11, 3.8, 4.2);
	s.target = point(0, 1.0, 1.5);
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

