#pragma once
#include"Vect.h"
class Ray {
public:
	
	Ray(Vect v1, Vect v2, double i,double r);	//contructor
	Ray();
	
	Vect getStart();
	Vect getDir();		// getter
	double getIntense();
	double getRefractive();
	
	void setStart(Vect v);
	void setDir(Vect v);			//setter
	void setIntense(double i);
	void setRefractive(double r);
private:
	Vect start;
	Vect dir;
	double intense;
	double ri;
};