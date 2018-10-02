#include"Box.h"

Cylinder::Cylinder()
{
	axis = Vect(0, 0, 0);
	max = 100000;
	min = -100000;
	col = Color(0, 0, 0);
	mat = Material();
}

Cylinder :: Cylinder(Vect &v, double zmax, double zmin,double r, Color c, Material m)
	{
		axis = v;
		max = zmax;
		min = zmin;
		col = c;
		mat = m;
		rad = r;
	}
Cylinder :: Cylinder(const Cylinder &v)
	{
		axis = v.axis;
		max = v.max;
		min = v.min;
		col = v.col;
		mat = v.mat;
		rad = v.rad;
	}
double Cylinder :: getT(Ray r)
{
	double a = pow(r.getDir().getX(),2) + pow(r.getDir().getZ(), 2);
	double b = 2.0 * (r.getStart().getX() - axis.getX())*r.getDir().getX() + 2.0 * (r.getStart().getZ() - axis.getZ())*r.getDir().getZ();
	double c = pow(r.getStart().getX() - axis.getX(), 2) + pow(r.getStart().getZ() - axis.getZ(), 2) - rad*rad;
	double d = b * b - 4 * a*c;
	if (d < 0)	return -1;
	double t1 = (-b + sqrt(d)) / 2.0;
	double t2 = (-b - sqrt(d)) / 2.0;
	if (t1 < 0 && t2 < 0) return -1;
	double ze1 = r.getStart().getY() + r.getDir().getY()*t1;
	double ze2 = r.getStart().getY() + r.getDir().getY()*t2;
	/*if ((t1 < t2 && t1>0) || (t1 > 0 && t2 < 0)) {
		double ze = r.getStart().getY() + r.getDir().getY()*t1;
		if (ze >= min && ze <= max)
			return t1;
		else return -1;
	}
	else if ((t2 <= t1 && t2 > 0) || (t2 > 0 && t1 < 0)) {
		double ze = r.getStart().getY() + r.getDir().getY()*t2;
		if (ze >= min && ze <= max)
			return t2;
		else
			return -1;
	}*/
	int flag1 = 0, flag2 = 0;
	if (ze1 >= min && ze1 <= max) flag1 = 1;
	if (ze2 >= min && ze2 <= max) flag2 = 1;
	if (flag1 == 0 && flag2 == 0)
		return -1;
	else if (flag1 == 1 && flag2 == 0)
		return t1;
	else if (flag1 == 0 && flag2 == 1)
		return t2;
	else
	{
		if (t1 < t2 && t1>0)
			return t1;
		else if (t2 < t1 && t2>0)
			return t2;
		else
			return -1;
	}
}

Vect Cylinder::getIntersect(Ray r)
{
	double a = pow(r.getDir().getX(), 2) + pow(r.getDir().getZ(), 2);
	double b = 2.0 * (r.getStart().getX() - axis.getX())*r.getDir().getX() + 2.0 * (r.getStart().getZ() - axis.getZ())*r.getDir().getZ();
	double c = pow(r.getStart().getX() - axis.getX(), 2) + pow(r.getStart().getZ() - axis.getZ(), 2) - rad*rad;
	double d = b * b - 4 * a*c;
	if (d < 0)	return Vect(-1, -1, -1);
	double t1 = (-b + sqrt(d)) / 2.0;
	double t2 = (-b - sqrt(d)) / 2.0;
	double ze1 = r.getStart().getY() + r.getDir().getY()*t1;
	double ze2 = r.getStart().getY() + r.getDir().getY()*t2;
	/*if ((t1 < t2 && t1>0) || (t1 > 0 && t2 < 0)) {
		double ze = r.getStart().getY() + r.getDir().getY()*t1;
		if (ze >= min && ze <= max)
			return Vect(r.getStart().getX() + t1 * (r.getDir().getX()), r.getStart().getY() + t1 * (r.getDir().getY()), r.getStart().getZ() + t1 * (r.getDir().getZ()));
		else return Vect(-1, -1, -1);
	}
	else if ((t2 <= t1 && t2 > 0) || (t2 > 0 && t1 < 0)) {
		double ze = r.getStart().getY() + r.getDir().getY()*t2;
		if (ze >= min && ze <= max)
			return Vect(r.getStart().getX() + t2 * (r.getDir().getX()), r.getStart().getY() + t2 * (r.getDir().getY()), r.getStart().getZ() + t2 * (r.getDir().getZ()));
		else
			return Vect(-1, -1, -1);
	}
	else return Vect(-1, -1, -1);*/

	int flag1 = 0, flag2 = 0;
	if (ze1 >= min && ze1 <= max) flag1 = 1;
	if (ze2 >= min && ze2 <= max) flag2 = 1;
	if (flag1 == 0 && flag2 == 0)
		return Vect(-1,-1,-1);
	else if (flag1 == 1 && flag2 == 0)
		return Vect(r.getStart().getX() + t1 * (r.getDir().getX()), r.getStart().getY() + t1 * (r.getDir().getY()), r.getStart().getZ() + t1 * (r.getDir().getZ()));
	else if (flag1 == 0 && flag2 == 1)
		return Vect(r.getStart().getX() + t2 * (r.getDir().getX()), r.getStart().getY() + t2 * (r.getDir().getY()), r.getStart().getZ() + t2 * (r.getDir().getZ()));
	else
	{
		if (t1 < t2 && t1>0)
			return Vect(r.getStart().getX() + t1 * (r.getDir().getX()), r.getStart().getY() + t1 * (r.getDir().getY()), r.getStart().getZ() + t1 * (r.getDir().getZ()));
		else if (t2 < t1 && t2>0)
			return Vect(r.getStart().getX() + t2 * (r.getDir().getX()), r.getStart().getY() + t2 * (r.getDir().getY()), r.getStart().getZ() + t2 * (r.getDir().getZ()));
		else
			return Vect(-1,-1,-1);
	}
}
Material Cylinder::getMaterial()
{
	return mat;
}
Vect Cylinder::getNormal(Vect v)
{
	return Vect(2 * (v.getX() - axis.getX()), 0, 2 * (v.getZ() - axis.getZ())).unitVector();
}
Color Cylinder::getColor()
{
	return col;
}