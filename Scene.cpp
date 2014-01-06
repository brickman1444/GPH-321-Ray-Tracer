
#include "time.h"
#include "scene.h"
#include "ray.h"
#include "win.h"

void scene::DrawScene(char *outputFileName)
{
	//width in pixels
	int w = win->Width();
	//height in pixels
	int h = win->Height();
	//width in space of a pixel
	double pixelW = windowW / w;
	//height in space of a pixel
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// Calculate the normalized view vector from the camera toward the target
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
	if (outputFileName != NULL) {
		cout << "Saved to " << outputFileName << endl;
		bit->Save(outputFileName);
	}
}

bool scene::CastRay(const ray &r, intersection &inter)
{
	bool found = false;
	intersection tmp;

	inter.t = DBL_MAX;	 // Make an impossibly large t

	shape *s;
	for (shapes.MoveFirst(); !shapes.AtEnd(); shapes.MoveNext())
	{
		s = shapes.GetCurrent();

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
	// Does the same as in DrawScene, but samples each pixel on a rectangular
	// grid evenly inside a pixel with xSamples * ySamples rays
	// and averages the results

	//width in pixels
	int w = win->Width();
	//height in pixels
	int h = win->Height();
	//width in space of a pixel
	double pixelW = windowW / w;
	//height in space of a pixel
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// Calculate the normalized view vector from the camera toward the target
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

	// Calculate the coordinates of upper left corner of the upper left pixel ( pixel(0, 0) )
	point upperLeft = center + left * windowW * .5 + up * windowH * .5;

	int t1 = clock();
	ray r;
	rgb result;

	//width in space of a subpixel
	double subpixelW = pixelW / xSamples;
	//height in space of a subpixel
	double subpixelH = pixelH / ySamples;

	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)	{
		for (int i = 0; i < w; i++) {
			// Calculate the position of the upper left corner of pixel(i, j) in space
			point screenP = upperLeft + -1 * left * i * pixelW + -1 * up * j * pixelH;

			//clear result so it can be used to sum up the subpixels
			result = rgb(0,0,0);

			for (int k = 0; k < ySamples; k++) {
				for (int l = 0; l < xSamples; l++) {
					point subpixelPoint = screenP + -1 * left * l * subpixelW + -1 * up * k * subpixelH;

					// Build the ray from the camera to the subpixel
					r.p = camera;
					r.v = subpixelPoint - camera;
					r.v.Normalize();

					// Trace the ray to get the color we see and add that to the result
					result += TraceRay(r);
				}
			}

			//divide by the number of samples to find the average of the color of the subpixels
			result /= (xSamples * ySamples);
			// clamp the color and write the result to the screen
			result.Clamp();

			bit->Point(i, j, result.R(), result.G(), result.B());							
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL) {
		cout << "Saved to " << outputFileName << endl;
		bit->Save(outputFileName);
	}
}

void scene::DrawSceneMonteCarlo(int samples, char *outputFileName)
{
	// Do the same as in DrawScene, but sample each pixel randomly
	// samples times and average the results

	//width in pixels
	int w = win->Width();
	//height in pixels
	int h = win->Height();
	//width in space of a pixel
	double pixelW = windowW / w;
	//height in space of a pixel
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// Calculate the normalized view vector from the camera toward the target
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

	// Calculate the coordinates of upper left corner of the upper left pixel ( pixel(0, 0) )
	point upperLeft = center + left * windowW * .5 + up * windowH * .5;

	int t1 = clock();
	ray r;
	rgb result;
	srand(t1);

	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)	{
		for (int i = 0; i < w; i++) {
			// Calculate the position of the upper left corner of pixel(i, j) in space
			point screenP = upperLeft + -1 * left * i * pixelW + -1 * up * j * pixelH;

			//clear result so it can be used to sum up the subpixels
			result = rgb(0,0,0);

			for (int k = 0; k < samples; k++) {
				double subpixelX = (double)rand() / RAND_MAX;
				double subpixelY = (double)rand() / RAND_MAX;
				point subpixelPoint = screenP + -1 * left * subpixelX * pixelW + -1 * up * subpixelY * pixelH;

				// Build the ray from the camera to the subpixel
				r.p = camera;
				r.v = subpixelPoint - camera;
				r.v.Normalize();

				// Trace the ray to get the color we see and add that to the result
				result += TraceRay(r);
			}

			//divide by the number of samples to find the average of the color of the subpixels
			result /= samples;
			// clamp the color and write the result to the screen
			result.Clamp();

			bit->Point(i, j, result.R(), result.G(), result.B());							
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL) {
		cout << "Saved to " << outputFileName << endl;
		bit->Save(outputFileName);
	}
}

void scene::DrawSceneMonteCarloAdvanced(int minSamples, int maxSamples, double differenceThreshold, char *outputFileName)
{
	// Works similarly to DrawSceneMonteCarlo. Samples the pixel minSmaples times, 
	// starting with the four corners and then for every further sample beyond minSamples, 
	// it checks the difference between the new average and the previous average. if the 
	// difference between the two is less than the differenceThreshold, it uses the new average. 

	// differenceThreshold is an empirically derived value that represents the distance between 
	// colors in 3D colorspace. This value ranges from 0, which means the two colors are the same
	// to sqrt(3) which is approximately 1.732 which is the distance from (0,0,0) to (1,1,1)

	// Set minSamples = 4 in order to at least samples each corner of a pixel
	if (minSamples < 4) {
		minSamples = 4;
	}

	//width in pixels
	int w = win->Width();
	//height in pixels
	int h = win->Height();
	//width in space of a pixel
	double pixelW = windowW / w;
	//height in space of a pixel
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// Calculate the normalized view vector from the camera toward the target
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

	// Calculate the coordinates of upper left corner of the upper left pixel ( pixel(0, 0) )
	point upperLeft = center + left * windowW * .5 + up * windowH * .5;

	int t1 = clock();
	ray r;
	rgb resultSum, previousResultSum;
	srand(t1);

	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)	{
		for (int i = 0; i < w; i++) {
			// Calculate the position of the upper left corner of pixel(i, j) in space
			point screenP = upperLeft + -1 * left * i * pixelW + -1 * up * j * pixelH;

			//clear result so it can be used to sum up the subpixels
			resultSum = rgb(0,0,0);
			int samples = 0;
			int finalNumberOfSamples;

			while (samples < 5) {
				point subpixelPoint;
				// sample the corners first in order to get a good sampling 
				// of the pixel and any possible edges going though it
				switch(samples) {
					case 0: //topLeft 
						subpixelPoint = screenP;
						break;
					case 1: //topRight 
						subpixelPoint = screenP + -1 * left * pixelW;
						break;
					case 2: //bottomLeft
						subpixelPoint = screenP + -1 * up * pixelH;
						break;
					case 3: //bottomRight
						subpixelPoint = screenP + -1 * left * pixelW + -1 * up * pixelH;
						break;
					case 4: //center
						subpixelPoint = screenP + -1 * left * pixelW + -1 * up * pixelH;
						break;
				}

				// Build the ray from the camera to the subpixel
				r.p = camera;
				r.v = subpixelPoint - camera;
				r.v.Normalize();

				if (samples < 4) {
					resultSum += TraceRay(r);
					samples++;
				} else {
					rgb averageCornerColor = resultSum / samples;
					rgb centerColor = TraceRay(r);
					resultSum += centerColor;
					samples++;

					// If the distance between the average corner values and the center value is within the threshold
					// then there probably isn't an edge within the pixel, so use fewer samples
					if (Distance(averageCornerColor, centerColor) < differenceThreshold) {
						finalNumberOfSamples = minSamples;	
					} else {
						finalNumberOfSamples = maxSamples;
					}
				}
			}

			while( samples < finalNumberOfSamples ) {
				
				//every sample past the fourth sample
				double subpixelX = (double)rand() / RAND_MAX;
				double subpixelY = (double)rand() / RAND_MAX;
				point subpixelPoint = screenP + -1 * left * subpixelX * pixelW + -1 * up * subpixelY * pixelH;

				// Build the ray from the camera to the subpixel
				r.p = camera;
				r.v = subpixelPoint - camera;
				r.v.Normalize();

				// Trace the ray to get the color we see and add that to the result
				resultSum += TraceRay(r);
				samples++;
			}

			//divide by the number of samples to find the average of the color of the subpixels
			resultSum /= samples;
			// clamp the color and write the result to the screen
			resultSum.Clamp();

			bit->Point(i, j, resultSum.R(), resultSum.G(), resultSum.B());							
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL) {
		cout << "Saved to " << outputFileName << endl;
		bit->Save(outputFileName);
	}
}

void scene::DrawSceneEdgeDetect(double differenceThreshold, char *outputFileName)
{
	// Works similarly to DrawSceneMonteCarloAdvanced. Colors edges that beat the threshold 
	// black and colors everything else white.

	// differenceThreshold is an empirically derived value that represents the distance between 
	// colors in 3D colorspace. This value ranges from 0, which means the two colors are the same
	// to sqrt(3) which is approximately 1.732 which is the distance from (0,0,0) to (1,1,1)

	//width in pixels
	int w = win->Width();
	//height in pixels
	int h = win->Height();
	//width in space of a pixel
	double pixelW = windowW / w;
	//height in space of a pixel
	double pixelH = windowH / h;

	if (bit != NULL)
		delete bit;
	bit = new Bitmap(*win, w, h);

	// Calculate the normalized view vector from the camera toward the target
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

	// Calculate the coordinates of upper left corner of the upper left pixel ( pixel(0, 0) )
	point upperLeft = center + left * windowW * .5 + up * windowH * .5;

	int t1 = clock();
	ray r;
	rgb resultSum, previousResultSum;
	srand(t1);

	// Now, loop through the pixels on the screen
	for (int j = 0; j < h; j++)	{
		for (int i = 0; i < w; i++) {
			// Calculate the position of the upper left corner of pixel(i, j) in space
			point screenP = upperLeft + -1 * left * i * pixelW + -1 * up * j * pixelH;

			//clear result so it can be used to sum up the subpixels
			resultSum = rgb(0,0,0);
			int samples = 0;

			while (samples < 5) {
				point subpixelPoint;
				// sample the corners first in order to get a good sampling 
				// of the pixel and any possible edges going though it
				switch(samples) {
					case 0: //topLeft 
						subpixelPoint = screenP;
						break;
					case 1: //topRight 
						subpixelPoint = screenP + -1 * left * pixelW;
						break;
					case 2: //bottomLeft
						subpixelPoint = screenP + -1 * up * pixelH;
						break;
					case 3: //bottomRight
						subpixelPoint = screenP + -1 * left * pixelW + -1 * up * pixelH;
						break;
					case 4: //center
						subpixelPoint = screenP + -1 * left * pixelW + -1 * up * pixelH;
						break;
				}

				// Build the ray from the camera to the subpixel
				r.p = camera;
				r.v = subpixelPoint - camera;
				r.v.Normalize();

				if (samples < 4) {
					resultSum += TraceRay(r);
					samples++;
				} else {
					rgb averageCornerColor = resultSum / samples;
					rgb centerColor = TraceRay(r);
					resultSum += centerColor;
					samples++;

					// If the distance between the average corner values and the center value is within the threshold
					// then there probably isn't an edge within the pixel, so color it white
					if (Distance(averageCornerColor, centerColor) < differenceThreshold) {
						bit->Point(i, j, rgb::white.R(), rgb::white.G(), rgb::white.B());	
					} else {
						bit->Point(i, j, rgb::black.R(), rgb::black.G(), rgb::black.B());	
					}
				}
			}						
		}
		CopySceneToWindow();
		Window::HandleEvents();
	}
	int t2 = clock();
	cout << "Frame drawn in " << double(t2-t1) / CLOCKS_PER_SEC << endl;
	if (outputFileName != NULL) {
		cout << "Saved to " << outputFileName << endl;
		bit->Save(outputFileName);
	}
}