
#include "time.h"
#include "scene.h"
#include "ray.h"
#include "win.h"

void scene::DrawScene(char *outputFileName)
{
	int w = win->Width();
	int h = win->Height();
	double pixelW = windowW / w;
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// To Do
	//
	// Calculate the normalized view vector
	vector view = target - camera;
	view.Normalize();

	// Calculate the center of the window
	point center = camera + view * windowD;

	// Before we can use the up vector, we need to modify up
	// so that it is perpendicular to the view vector, and
	// normalize it.
	up = up.Perp(view);
	up.Normalize();

	// Now calculate the left vector.
	vector left = Cross(up,view);

	// Calculate the coordinates of upperLeft = pixel(0, 0)
	point upperLeft = center + left * windowW * .5 + up * windowH * .5;

	int t1 = clock();
	ray r;
	rgb result;
	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)	{
		for (int i = 0; i < w; i++) {
			// Calculate the position of pixel(i, j) in space
			point screenP = upperLeft + -1 * left * i * pixelW + -1 * up * j * pixelH;

			// Then build the ray from the camera to the pixel
			//ray r;
			r.p = camera;
			r.v = screenP - camera;
			r.v.Normalize();

			// Finally, we trace the ray to get the color we see,
			// clamp the color and write the result to the screen
			//rgb result = TraceRay(r);
			result = TraceRay(r);
			result.Clamp();
			bit->Point(i, j, result.R(), result.G(), result.B());							
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL)
		bit->Save(outputFileName);
}

bool scene::CastRay(const ray &r, intersection &inter)
{
	bool found = false;
	intersection tmp;

	inter.t = DBL_MAX;	 // Make an impossibly large t

	shape *s;
	for (shapes.MoveFirst(); !shapes.AtEnd(); shapes.MoveNext())
	{
		//shape *s = shapes.GetCurrent();
		s = shapes.GetCurrent();

		// To Do
		//
		// We loop through the shapes in the scene's "shapes" list
		// and calculate the intersection of the ray with the current
		// shape with the shape's "CalculateIntersection" method.
		// 
		// If there was an intersection (check the return value), 
		// then check if the temporary intersection t is lower than
		// inter.t.  If it is, copy the data into "inter", and set
		// the "world" pointer of the intersection to "this" scene.

		if (s->CalculateIntersection(r, tmp)) {
			if (tmp.t < inter.t) {
				inter = tmp;
				/*inter.diff = tmp.diff;
				inter.n = tmp.n;
				inter.p = tmp.p;
				inter.shiny = tmp.shiny;
				inter.spec = tmp.spec;
				inter.t = tmp.t;
				inter.tex = tmp.tex;
				inter.v = tmp.v;*/
				inter.w = this;
				found = true;
			}
		}
	}
	return found;
}

rgb scene::TraceRay(const ray &r)
{
	intersection inter;	
	if (CastRay(r, inter) == true)
		return inter.s->ApplyMaterial(inter, lights, shapes);
	else
		return backgroundColor;			
}

void scene::CopySceneToWindow(void)
{
	if (bit)
		bit->Copy(*win, 0, 0);
}

/////////////////////////////////////////////////////////////////////////////////////
// The following are advanced antialiased versions of the DrawScene method
/////////////////////////////////////////////////////////////////////////////////////

void scene::DrawSceneAntialias(int xSamples, int ySamples, char *outputFileName)
{
	// To Do
	// 
	// Do the same as in DrawScene, but sample each pixel on a rectangular
	// grid evenly inside a pixel with xSamples * ySamples rays
	// and average the results
}

void scene::DrawSceneMonteCarlo(int samples, char *outputFileName)
{
	// To Do
	// 
	// Do the same as in DrawScene, but sample each pixel randomly
	// samples times and average the results
}

