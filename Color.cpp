#include "Color.h"


Color::Color()
{
	r = 0.5;
	g = 0.5;
	b = 0.5;
}

Color::Color(double red, double green, double blue)
{
	r = red;
	g = green;
	b = blue;
}

Color::Color(const Color & c)
{
	r = c.r;
	b = c.b;
	g = c.g;
}

double Color::getR()
{
	return r;
}

double Color::getB()
{
	return b;
}

double Color::getG()
{
	return g;
}

void Color::setR(double i)
{
	if (i > 1)
		r = 1;
	else
	r = i;
}

void Color::setG(double i)
{
	if (i > 1)
		g = 1;
	else
	g = i;
}

void Color::setB(double i)
{
	if (i > 1)
		b = 1;
	else
	b = i;
}

Color Color::colMuly(Color c)
{
	return Color((r+c.r)/2, (g + c.g) / 2, (b + c.b) / 2);
}

Color Color::scalarmult(double i)
{
	return Color(r*i,g*i,b*i);
}

bool Color::equals(Color c)
{
	if (c.r == r && c.g == g && c.b == b) return true;
	return false;
}

double Color::getIntensityFromOriginalColor()
{
	if (r != 0) return r;
	if (g != 0) return g;
	if (b != 0) return b;
}

