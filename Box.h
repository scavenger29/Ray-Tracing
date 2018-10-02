#include "WorldObject.h"
#pragma once

class Cylinder
{
public:
	Cylinder();
	Cylinder(Vect &v,double zmax,double zmin,double radius, Color c, Material m);
	Cylinder(const Cylinder &v);
	Vect axis;
	double max;
	double min;
	double rad;
	Material mat;
	Color col;
	double getT(Ray r);
	Vect getIntersect(Ray r);
	Material getMaterial();
	Vect getNormal(Vect v);
	Color getColor();
};
