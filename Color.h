#ifndef __COLOR_H__
#define __COLOR_H__

#include "jmisc.h"
#include "MiscMath.h"

class rgb
{
public:

	rgb(double R = 0.0, double G = 0.0, double B = 0.0) 
	{ Set(R, G, B); }

	void Set(double R, double G, double B)
	{ SetR(R); SetG(G); SetB(B); }

	void SetR(double R) { r = R; }
	void SetG(double G) { g = G; }
	void SetB(double B) { b = B; }

	double R(void) const { return r; }
	double G(void) const { return g; }
	double B(void) const { return b; }

	void Clamp(void)
		{ r = clamp(r, 0.0, 1.0); g = clamp(g, 0.0, 1.0); b = clamp(b, 0.0, 1.0); }
	void multiply(rgb &res, const rgb &c1, const rgb &c2)
		{ res.SetR(c1.r * c2.r); res.SetG(c1.g * c2.g); res.SetB(c1.b * c2.b); }
	void add(rgb &res, const rgb &c1, const rgb &c2)
		{ res.SetR(c1.r + c2.r); res.SetG(c1.g + c2.g); res.SetB(c1.b + c2.b); }

	rgb &operator*=(double d);
	rgb &operator*=(const rgb &c1);
	rgb &operator+=(const rgb &c1);

	friend rgb operator*(const rgb &c, double d);
	friend rgb operator*(double d, const rgb &c);
	friend rgb operator*(const rgb &c1, const rgb &c2);
	friend rgb operator+(const rgb &c1, const rgb &c2);
	
	// Useful color definitions
	static rgb white;		// Grays
	static rgb lightGray;
	static rgb mediumGray;
	static rgb gray;
	static rgb darkGray;
	static rgb black;
	static rgb red;			// Primaries
	static rgb green;
	static rgb blue;
	static rgb cyan;		// Secondaries
	static rgb magenta;
	static rgb fuchsia;
	static rgb yellow;
	static rgb azure;		// Tertiaries
	static rgb violet;
	static rgb rose;
	static rgb orange;
	static rgb chartreuse;
	static rgb springGreen;
	static rgb brown;		// Misc colors
	static rgb tan;
	static rgb beige;
	static rgb skyBlue;
	static rgb offWhite;	// Same as "Floral White", slightly yellowish

private:

	double r, g, b;
};

inline ostream &operator<<(ostream &output, const rgb &c)
{
	output << c.R() << ", " << c.G() << ", " << c.B() << ")" << endl;
	return output;
}

#endif