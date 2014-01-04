#pragma warning(disable : 4996) // Turns off secure_crt warnings in VS 2005 and up

#include <math.h>
#include <jmisc.h> // Miscellaneous routines, including sqr()
#include "vector.h"

////////////////////////////////////////////////////////////////////////////
// Methods of the vector class
////////////////////////////////////////////////////////////////////////////

vector::vector(double x, double y, double z)
{
	v[0] = x; 
	v[1] = y; 
	v[2] = z;
	v[3] = 0;
}

double vector::Length(void) const 
{
	// Return the length of this vector
	//return sqrt(sqr(AsArray()[0])+sqr(AsArray()[1])+sqr(AsArray()[2]));
	return sqrt(v[0]*v[0]+v[1]*v[1]+v[2]*v[2]);
}

void vector::Normalize(void)
{
	// Normalize this vector, but make sure to check whether the vector is zero or not
	// Check if it is the zero vector.
	double len = Length();
	assert(IsNotZero(len));
	AsArray()[0] /= len;
	AsArray()[1] /= len;
	AsArray()[2] /= len;
}

// This operator has been provided for you as an example implementation
vector &vector::operator+=(const vector &w)
{
	// Remember, v[] is the internal array in this class and is accessible from within this function
	// because it is a method of the class!

	v[0] += w[0]; 
	v[1] += w[1]; 
	v[2] += w[2];

	return *this;
}

vector &vector::operator-=(const vector &w)
{
	// Subtract the vector "w" from this vector.  
	v[0] -= w[0]; 
	v[1] -= w[1]; 
	v[2] -= w[2];
	return *this;
}

vector &vector::operator*=(double d)
{
	// Multiply this vector by the scalar d.  
	v[0] *= d; 
	v[1] *= d; 
	v[2] *= d; 
	return *this;
}

vector &vector::operator/=(double d)
{
	// Divide this vector by the scalar d, make sure to check to see if it can be done  
	assert(IsNotZero(d));
	v[0] /= d; 
	v[1] /= d; 
	v[2] /= d; 
	return *this;
}

// Calculates the projection of v onto w
vector vector::Proj(const vector &w)
{
	// Calculate the projection of "this" vector onto the vector w

	double len = w.Length();

	assert(IsNotZero(len));

	return (Dot(*this,w)/len/len) * w;
}

vector vector::Perp(const vector &w)
{
	// Calculate the projection of "this" vector perpendicular to the vector w

	return *this - Proj(w);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  The following are not methods of the class, rather they are global functions and operators that work
//  on the class.  For convenience, the class has been provided with an indexing [] operator to allow you to 
//  use the vector object just like an array.  If you have a vector "w", then w[0] is the x-component of w.
///////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool IsZero(const vector &v)
{
	// Return true if the vector is a zero vector, otherwise return false

	return IsEqual(v[0],0) && IsEqual(v[1],0) && IsEqual(v[2],0);
}

bool IsEqual(const vector &v, const vector &w)
{
	// Return true if the two vectors are equal, otherwise return false
	return IsEqual(v[0],w[0]) && IsEqual(v[1],w[1]) && IsEqual(v[2],w[2]);
}

bool IsNotEqual(const vector &v, const vector &w)
{ 
	return !IsEqual(v, w); 
}

double Dot(const vector &v, const vector &w)
{
	// Return the dot product of v and w

	return v[0]*w[0]+v[1]*w[1]+v[2]*w[2];
}

vector Cross(const vector &v, const vector &w)
{
	// Return the vector cross product of v and w

	return vector(v[1]*w[2]-v[2]*w[1], v[2]*w[0]-v[0]*w[2], v[0]*w[1]-v[1]*w[0]);
}

vector operator+(const vector &v, const vector &w)
{
	// Return the sum of v and w
	return vector(v[0]+w[0], v[1]+w[1], v[2]+w[2]);
}

vector operator-(const vector &v, const vector &w)
{
	// Return the difference of v and w
	return vector(v[0]-w[0], v[1]-w[1], v[2]-w[2]);
}

vector operator*(double d, const vector &v)
{
	// To Do
	//
	// Return the product of d and v

	return vector(v[0]*d, v[1]*d, v[2]*d);
}

vector operator*(const vector &v, double d)
{
	return d * v;
}

vector operator/(const vector &v, double d)
{
	// To Do
	//
	// Return the quotient of v and d.  Don't forget to check if d is zero.
	assert(IsNotEqual(d,0));
	return vector(v[0]/d, v[1]/d, v[2]/d);
}

point operator+(const point &p, const vector &v)
{
	// To Do
	// 
	// Return the sum of p and v
	assert(IsNormal(p));
	return point(p[0]+v[0], p[1]+v[1], p[2]+v[2]);
}

// Note that this one really doesn't make sense geometrically, 
// but sometimes we define certain operators for symmetry and 
// for ease of programming.  But you should be careful to see
// what the operation means when you do it.
point operator+(const vector &v, const point &p)
{
	assert(IsNormal(p));
	return p + v;
}

point &operator+=(point &p, const vector &v)
{
	// To Do
	//
	// Add v to the point p and then return it
	assert(IsNormal(p));
	p = point(p[0] + v[0],p[1]+v[1],p[2]+v[2]);
	return p;
}

point operator-(const point &p, const vector &v)
{
	// To Do
	// 
	// Return p - v
	assert(IsNormal(p));
	return point(p[0]-v[0], p[1]-v[1], p[2]-v[2]);
}

point &operator-=(point &p, const vector &v)
{
	// Subtract the vector v from p, and return p.
	assert(IsNormal(p));
	p = point(p[0]-v[0],p[1]-v[1],p[2]-v[2]);
	return p;
}

vector operator-(const point &p, const point &q)
{
	// Subtract the two points, p and q
	assert(IsNormal(p)&&IsNormal(q));
	return vector(p[0]-q[0], p[1]-q[1], p[2]-q[2]);
}

vector operator-(const vector &v)
{
	// To Do
	//
	// Return the negative of v

	return vector(-v[0], -v[1], -v[2]);
}

istream &operator>>(istream &input, vector &v)
{
	input >> v[0] >> v[1] >> v[2];
	return input;
}

ostream &operator<<(ostream &output, const vector &v)
{
	output << v[0] << " " << v[1] << " " << v[2];
	return output;
}
