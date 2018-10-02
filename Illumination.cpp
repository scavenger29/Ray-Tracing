#include"Illumination.h"
#include "WorldObject.h"
#include"Vect.h"
#include"Sphere.h"
#include "Box.h"
#include<cmath>
#include<iostream>
using namespace std;

Illumination::Illumination()
{
	pos = Vect(); intensity = 0;
}

Illumination::Illumination( Vect v,double i)
{
	intensity = i;
	pos = v;
}

double Illumination::getIntensityAtIntersectSphere(Sphere obj, Vect intPoint, Vect viewer)
{	
	Vect light = pos.minus(intPoint).unitVector();
	Vect nor = obj.getNormal(intPoint);
	double diffuse = obj.getMaterial().getDifCoff() * intensity * (light.dot(nor));
	if (diffuse < 0) diffuse = 0;
	
	Vect reflect = light.minus(nor.scalarMult(2 *(light.dot(nor)))).unitVector();
	reflect = reflect.opposite();
	Vect view = viewer.minus(intPoint).unitVector();
	double specular = obj.getMaterial().getSpecCoff() * intensity * pow( reflect.dot(view), obj.getMaterial().getSpecPow());
	if (specular < 0)	specular = 0;

	double totalIntensity = diffuse + specular;
	return totalIntensity;
}

double Illumination::getIntensityAtIntersectPlane(Plane obj, Vect intPoint, Vect viewer)
{
	Vect light = intPoint.minus(pos).unitVector();
	Vect nor = obj.getNormal(intPoint);
	double diffuse = obj.getMaterial().getDifCoff() * intensity * (light.opposite().dot(nor));
	if (diffuse < 0) diffuse = 0;

	Vect reflect = light.minus(nor.scalarMult(2 *(light.dot(nor)))).unitVector();
	Vect view = viewer.minus(intPoint).unitVector();
	double specular = obj.getMaterial().getSpecCoff() * intensity * pow(reflect.dot(view), obj.getMaterial().getSpecPow());
	if (specular < 0)	specular = 0;

	double totalIntensity = diffuse + specular;
	return totalIntensity;
}

double Illumination::getIntensityAtIntersectPolygon(Poly obj, Vect intPoint, Vect viewer)
{
	Vect light = intPoint.minus(pos).unitVector();
	Vect nor = obj.getNormal(intPoint);
	double diffuse = obj.getMaterial().getDifCoff() * intensity * (light.opposite().dot(nor));
	if (diffuse < 0) diffuse = 0;

	Vect reflect = light.minus(nor.scalarMult(2 *(light.dot(nor)))).unitVector();
	Vect view = viewer.minus(intPoint).unitVector();
	double specular = obj.getMaterial().getSpecCoff() * intensity * pow(reflect.dot(view), obj.getMaterial().getSpecPow());
	if (specular < 0)	specular = 0;

	double totalIntensity = diffuse + specular;
	return totalIntensity;
}
double Illumination::getIntensityAtIntersectCylinder(Cylinder obj, Vect intPoint, Vect viewer)
{
	Vect light = intPoint.minus(pos).unitVector();
	Vect nor = obj.getNormal(intPoint);
	double diffuse = obj.getMaterial().getDifCoff() * intensity * (light.opposite().dot(nor));
	if (diffuse < 0) diffuse = 0;

	Vect reflect = light.minus(nor.scalarMult(2 * (light.dot(nor)))).unitVector();
	Vect view = viewer.minus(intPoint).unitVector();
	double specular = obj.getMaterial().getSpecCoff() * intensity * pow(reflect.dot(view), obj.getMaterial().getSpecPow());
	if (specular < 0)	specular = 0;

	double totalIntensity = diffuse + specular;
	return totalIntensity;
}

Vect Illumination::getPos()
{
	return pos;
}

double Illumination::getIntensity()
{
	return intensity;
}

