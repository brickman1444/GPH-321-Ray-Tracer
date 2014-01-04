
#include "shape.h"
#include "material.h"
#include "light.h"
#include "scene.h"

/////////////////////////////////////////////////////////////////////
// Base class method for reflecting lights loops through all lights
// if a material doesn't use lights, it should override this
/////////////////////////////////////////////////////////////////////
rgb material::ReflectLights(intersection &inter, LinkedList<light *> &lights, LinkedList<shape *> &shapes)
{
	rgb totalAmb = rgb::black, totalDiff = rgb::black, totalSpec = rgb::black;

	ListIter<light *> lightIter(lights);
	for (lightIter.MoveFirst(); !lightIter.AtEnd(); lightIter.MoveNext())
	{
		lightOutput lightVals;
		rgb matAmb, matDiff, matSpec;

		lightIter.GetCurrent()->Illuminance(inter, lightVals);
		ReflectedColor(matAmb, matDiff, matSpec, lightVals, inter);

		totalAmb  += matAmb;
		totalDiff += matDiff;
		totalSpec += matSpec;
	}
	rgb result = totalAmb + totalDiff + totalSpec;
	return result;
}

////////////////////////////////////////////////////////////////////////////////////
// Constant material returns a constant color.  It overrides Reflect lights 
// because it ignores the lights
////////////////////////////////////////////////////////////////////////////////////
rgb constantMaterial::ReflectLights(intersection &inter, LinkedList<light *> &lights, LinkedList<shape *> &shapes)
{
	rgb totalAmb = rgb::black, totalDiff = rgb::black, totalSpec = rgb::black;

	lightOutput lightVals;
	ReflectedColor(totalAmb, totalDiff, totalSpec, lightVals, inter);
	rgb result = totalAmb + totalDiff + totalSpec;
	return result;
}

void constantMaterial::ReflectedColor(rgb &outA, rgb &outD, rgb &outS, const lightOutput &lightVal, 
									  const intersection &inter)
{
	outA = inter.diff;
	outD = rgb::black;
	outS = rgb::black;
}

////////////////////////////////////////////////////////////////////////////////////
// Fake Material simulates a light from the viewer's position.  It is derived from 
// constant material so that it ignores any real lights in the scene.  
////////////////////////////////////////////////////////////////////////////////////
void fakeMaterial::ReflectedColor(rgb &outA, rgb &outD, rgb &outS, const lightOutput &lightVal, 
								  const intersection &inter)
{
	outA = rgb::black;
	outD = inter.diff * Dot(inter.n, inter.v);
	outS = rgb::black;
}

/////////////////////////////////////////////////////////////////////////////////////
// Lambertian material only reflects diffuse and ambient light
/////////////////////////////////////////////////////////////////////////////////////
void lambertianMaterial::ReflectedColor(rgb &outA, rgb &outD, rgb &outS, const lightOutput &lightVal, 
										const intersection &inter)
{
	outA = rgb::black;
	outD = inter.diff;
	outS = rgb::black;

	// Assume that these are all outgoing from the surface and are already normalized 
	vector L = lightVal.L;		
	vector N = inter.n;			
	vector V = inter.v;			

	// to do: Get the forward-facing normal
	if (Dot(N,V) < 0) {
		N = -1*N;
	}

    // to do: calculate ambient reflection from ambient light source and diffuse reflectance
	outA = lightVal.a * inter.diff;

	// to do: calculate the dot product between L and N
	double LdotN = Dot(L,N);
	// to do:  calculate diffuse reflection  

	if (LdotN > 0) {
		outD = lightVal.d * inter.diff * LdotN;
	} else {
		outD = rgb::black;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Phong specular reflection material
////////////////////////////////////////////////////////////////////////////////////////////////
void phongMaterial::ReflectedColor(rgb &outA, rgb &outD, rgb &outS, 
								   const lightOutput &lightVal, const intersection &inter)
{
	outA = rgb::black;
	outD = rgb::black;   
	outS = rgb::black;

	// Assume that these are all outgoing from the surface and are already normalized 
	vector L = lightVal.L;		
	vector N = inter.n;			
	vector V = inter.v;			

	// to do: Get the forward-facing normal
	if (Dot(N,V) < 0) {
		N = -1*N;
	}

    // to do: calculate ambient reflection from ambient light source and diffuse reflectance
	outA = lightVal.a * inter.diff;

	// to do: calculate the dot product between L and N
	double LdotN = Dot(L,N);
	// to do:  calculate diffuse reflection  

	// to do: calculate the reflection R  of L about N
	// if N and L are unit then so is R
	vector R = -1*L + 2*L.Proj(N);

	// to do:  calculate the dot product of R and V
	double RdotV = Dot(R, V);

	// to do:  calculate the specular reflection -- Don't highlight the dark side
	// to do:  calculate diffuse reflection
	if (LdotN > 0) {
		outD = lightVal.d * inter.diff * LdotN;
	} else {
		outD = rgb::black;
	}

	if (RdotV > 0) {
		outS = lightVal.s * inter.spec*pow(RdotV,inter.shiny);
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////
// Half-way specular reflection material
////////////////////////////////////////////////////////////////////////////////////////////////
void blinnMaterial::ReflectedColor(rgb &outA, rgb &outD, rgb &outS, 
								   const lightOutput &lightVal, const intersection &inter)
{
	outA = rgb::black;
	outD = rgb::black;
	outS = rgb::black;

	// Assume that these are all outgoing from the surface and are already normalized 
	vector L = lightVal.L;		
	vector N = inter.n;			
	vector V = inter.v;			

	// to do: Get the forward-facing normal
	if (Dot(N,V) < 0) {
		N = -1*N;
	}

    // to do: calculate ambient reflection from ambient light source and diffuse reflectance
	outA = lightVal.a * inter.diff;

	// to do: calculate the dot product between L and N
	double LdotN = Dot(L,N);
 
    //  to do: Calculate the half-angle vector and normal
	vector H = L + V;
	H.Normalize();

	double HdotN = Dot(H,N);

	if (LdotN > 0) {
		// to do:  calculate diffuse reflection
		outD = lightVal.d * inter.diff * LdotN;
		//  to do: Compute specular reflection from half-angle vector
		outS = lightVal.s * inter.spec*pow(HdotN,inter.shiny);
	}
}

///////////////////////////////////////////////////////////////////////////////////
// A simple striped material derived from phong
///////////////////////////////////////////////////////////////////////////////////
void stripedPhong::ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
									   const lightOutput &lightVal, const intersection &inter)
{
	intersection tmp = inter;
	int ix;

	ix = int(fmod(inter.p[0], 2*stripeWidth) / (stripeWidth));

	// Override the surface color in the intersection data
	if (ix % 2 == 0)
		tmp.diff = color1;
	else
		tmp.diff = color2;

	phongMaterial::ReflectedColor(outA, outD, outS, lightVal, tmp);
}

///////////////////////////////////////////////////////////////////////////////////
// A simple checkerboard material derived from phong
///////////////////////////////////////////////////////////////////////////////////
void checkerboardPhong::ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
									   const lightOutput &lightVal, const intersection &inter)
{
	outA = rgb::black;
	outD = rgb::tan;  
	outS = rgb::black; 

	intersection tmp = inter;
	int ix = 0;

	// x,y,z
	// length,width,height
	ix = int(fmod(inter.p[0], 2*checkL) / (checkL));
	ix += int(fmod(inter.p[1], 2*checkW) / (checkW));

	if (inter.p[0] < 0) {
		ix++;
	}

	if (inter.p[1] < 0) {
		ix++;
	}

	// 3D checkerboard. Looks kinda bad on a sphere.
	// Leave commented out to get only a planar 
	// checkerboard from above with vertical stripes on vertical planes
	//ix += int(fmod(inter.p[2], 2*checkH) / (checkH));

	// Override the surface color in the intersection data
	if (ix % 2 == 0)
		tmp.diff = cDark;
	else
		tmp.diff = cLight;

	phongMaterial::ReflectedColor(outA, outD, outS, lightVal, tmp);
}

///////////////////////////////////////////////////////////////////////////////////
// A simple image texturing material derived from phong.  It uses the texture
// coordinates to look up a color from a texture image
///////////////////////////////////////////////////////////////////////////////////
double cycle(double val, double period)
{
	double z = fmod(val, period);
	if (z > 0)
		return z;
	else
		return z + period;
}

texturedPhong::texturedPhong(char *filename)
{
	LoadTexture(filename);	
}

void texturedPhong::LoadTexture(char *filename)
{
	texture.Read(filename);
}

void texturedPhong::ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
								   const lightOutput &lightVal, const intersection &inter)
{
	intersection tmp = inter;
	double r, g, b;

	texture.GetPixelLinear(cycle(inter.tex[0], 1.0) * (texture.Width() - 1), cycle(inter.tex[1], 1.0) * (texture.Height() - 1), r, g, b);
	tmp.diff = rgb(r, g, b);  // Override the surface color in the intersection data

	phongMaterial::ReflectedColor(outA, outD, outS, lightVal, tmp);
}

///////////////////////////////////////////////////////////////////////////////////
// reflectivePhong material calculates a mirror reflection and adds it to the
// specular component of phong
///////////////////////////////////////////////////////////////////////////////////

static int maxLevel = 10;
static int level = 0;

void MaterialSetMaxReflectionLevel(int k)
{
	maxLevel = k;
}

reflectivePhong::reflectivePhong(scene &S, double RI)
{
	SetScene(S);
	reflectiveIndex = RI;
}

void reflectivePhong::ReflectedColor(rgb& outA, rgb &outD, rgb &outS, 
									 const lightOutput &lightVal, const intersection &inter)
{
	phongMaterial::ReflectedColor(outA, outD, outS, lightVal, inter);

	if (world != NULL && level < maxLevel)
	{
		level++;

		// Calculate the reflection ray 
		ray ref;
		ref.v = -inter.v + 2 * Dot(inter.v, inter.n) * inter.n;
		ref.p = inter.p + ref.v * 1e-6;

		// Call trace on that reflection ray to find the color 
		// seen in the reflection. Multiply this color by 
		// the "reflectiveIndex" and add it to the specular
		// output color.
		rgb reflectedColor = world->TraceRay(ref);
		outS += reflectiveIndex * reflectedColor;

		level--;
	}
}

///////////////////////////////////////////////////////////////////////////////////////
// glass material calculates both reflection and refraction using the full Fresnel
// formula.  The reflectance and refraction terms are added to the specular reflection
///////////////////////////////////////////////////////////////////////////////////////

glass::glass(scene &S)
{
	SetScene(S);
}

void glass::ReflectedColor(rgb& outA, rgb &outD, rgb &outS, const lightOutput &lightVal, const intersection &inter)
{
	rgb diffuse, specular;
	shape *shp = inter.s;

	outA = rgb::black;
	outD = rgb::black;   
	outS = rgb::black;

	// Assume that these are all outgoing from the surface and are already normalized 
	vector L = lightVal.L;		// L is outgoing
	vector N = inter.n;			// N is outward facing
	vector V = inter.v;			// V is outgoing

	// Get the forward-facing normal
	N = faceforward(N, V);

   // to do:  Compute initial values of outA, outD and outS, as you did for Phong			

    // to do: calculate ambient reflection from ambient light source and diffuse reflectance
	outA = lightVal.a * inter.diff;

	// to do: calculate the dot product between L and N
	double LdotN = Dot(L,N);
	// to do:  calculate diffuse reflection  

	// to do: calculate the reflection R  of L about N
	// if N and L are unit then so is R
	vector R = -1*L + 2*L.Proj(N);

	// to do:  calculate the dot product of R and V
	double RdotV = Dot(R, V);

	// to do:  calculate the specular reflection -- Don't highlight the dark side
	// to do:  calculate diffuse reflection
	if (LdotN > 0) {
		outD = lightVal.d * inter.diff * LdotN;
	}

	if (RdotV > 0) {
		outS = lightVal.s * inter.spec*pow(RdotV,inter.shiny);
	}

	if (world != NULL && level < maxLevel)
	{
		level++;

		rgb transmitRayResult, reflectRayResult;

		// Assume that these are all outgoing from the surface and are already normalized 
		V = inter.v;
		N = inter.n;			

		// to do:  compute the position of the intersection
		//         reflect the reflection of v about the surface normal  
		ray reflectRay;
		reflectRay.v = reflect(N, V);
		reflectRay.p = inter.p + reflectRay.v*1e-6;

		// coefI:  Index of refraction of material that the ray is leaving
		// coefT:  Index of refraction of material that transmitted ray will enter
		double coefI, coefT;

		// to do:  When a ray leaves an object, it's pointing in the same direction
		//         as the surface normal.  Set coefI to object's index of refraction and
		//         coefT to the ambient (air) index of refraction:
		//            coefI = ambientN
		//            coefT = materialN
		//         when a ray enters an object, the coefficients are reversed.
		//
		//         Need this to implement Snell's law
	    //

		// V points back to the source. If V and N point in the same direction, 
		// then the ray is entering the object
		if (Dot(V,N) > 0) 
		{
			coefI = ambientN;
			coefT = materialN;
		} else {
			coefI = materialN;
			coefT = ambientN;
		}

		// to do:  compute normal in direction of v
		vector Nv = faceforward(N,V);
		// to do:  compute the (acute) angle between Nv and V
		double thetaI;

		thetaI = acos(Dot(Nv,V));

		// to do:  replace current assignment statement for sinThetaT
		//         with application of Snell's law to compute the sine of the angle
		//         of the transmitted ray 
		double sinThetaT = coefI / coefT * sin(thetaI);

		double fkR, fkT;

		if (fabs(sinThetaT) < .99999)  // Then thetaT will be < 90 degrees
		{
			// to do:  compute corresponding angle of sinThetaT
			double thetaT = asin(sinThetaT);

			// to do:  normalize the part of V that's perpendicular to Nv 
			vector tangentV = V.Perp(Nv); 
			tangentV.Normalize();
			// to do: compute the point of intersection and 
			//        direction of the transmitted ray
			ray transmitRay;
			transmitRay.v = -tangentV * sin(thetaT) - N * cos(thetaT);
			transmitRay.p = inter.p + transmitRay.v*1e-6;
			// to do:  compute Reflective Intensity (fkR) and 
			//         Transparent Intensity (fkT) according to
			//         Fresnel's equations
			double F = .5 * (sqr(tan(thetaI - thetaT)) / sqr(tan(thetaI + thetaT)) + sqr(sin(thetaI - thetaT)) / sqr(sin(thetaI + thetaT)));
			fkR = F * kR;
			fkT = sqrt((1-F*F)*((coefI*cos(thetaI)/(coefT*cos(thetaT)))))*kT;
			//cout << fkR << " " << fkT << endl;
			
			// to do:  Uncomment the folowing once proper values are computed for
			//         fkT and transmitRay
			if (IsNotZero(fkT))
				transmitRayResult = fkT * world->TraceRay(transmitRay);
			else
				transmitRayResult = rgb::black;
		}
		else			// Total internal reflection ... thetaT = 90 degrees ... no transmission
		{
			//to do:  uncomment the following when coefI and coefT are properly set
			fkR = sqr(coefI - coefT)/sqr(coefI + coefT);
			transmitRayResult = rgb::black;
		}
		//
		// uncomment the following onces fkR and reflectRay are properly set
		if (IsNotZero(fkR))
			reflectRayResult = fkR * world->TraceRay(reflectRay);
		else
			reflectRayResult = 0.0;

		outS += reflectRayResult + transmitRayResult * transmitColor;
		level--;
	}
}
