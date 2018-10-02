#include "Dependencies\glew\glew.h"
#include "Dependencies\freeglut\freeglut.h"
#include <iostream>
#include <vector>
#include<algorithm>
#include<cmath>
#include "Vect.h"
#include "Ray.h"
#include "CameraCoordSys.h"
#include "Color.h"
#include "Illumination.h"
#include "json/json.h"
#include "json/json-forwards.h"
#include <fstream>
#include "Material.h"
#include "WorldObject.h"
#include "Sphere.h"
#include "Plane.h"
#include "Poly.h"
#include "Box.h"
#include <list>
#include "json/json.h"


using namespace std;

//Json::Reader reader;
//Json::Value root;
//Json::StyledStreamWriter writer;
//ifstream ifile("worldInput.json", std::ifstream::binary);
//ifile>>root;
int levelOfTracing = 2;

// Set up Camera
CameraCoordSys vcs(Vect(250, 250, 200), Vect(0, 1, 0), Vect(0, 0, -1));
Vect eye = vcs.VCS2WCS(Vect(100, 50, -300));

//Anti-aliasing factor
int antiAliasingFactor = 4;

//Spheres
Sphere s1 = Sphere(Vect(00,440, 0), 60, Color(1.0, 1.0, 1.0), Material(0.01, 0.005, 0.1, 1.0 , 1.0 , 1.0, 1.5));
Sphere s2 = Sphere(Vect(0, 300, 50), 80, Color(1, 1, 1), Material(0.01, 0.015, 0.1, 1.0, 1.0, 1.0));

//Planes
Plane p1 = Plane(Vect(0, 180, 0), Vect(0, 1, 0), Color(0.0, 0.27, 0.07), Material(0.01, 0.01, 0.1, 1,0,0)); //down
Plane p2 = Plane(Vect(0, 800, 0), Vect(0,-1,0), Color(0.54, 0.27, 0.07), Material(0.01, 0.01, 0.1, 1,0,0)); //up
Plane p3 = Plane(Vect(0, 0, -120), Vect(0, 0, 1), Color(0.54, 0.27, 0.07), Material(0.01, 0.01, 0.1, 1,0,0));
Plane p4 = Plane(Vect(-400, 0, 0), Vect(1, 0, 0), Color(0.54, 0.27, 0.07), Material(0.01, 0.01, 0.1, 1,0,0));//left
Plane p5 = Plane(Vect(900, 0, 0), Vect(-1, 0, 0), Color(0.54, 0.27, 0.07), Material(0.01, 0.01, 0.1, 1,0,0));//right

Cylinder c1 = Cylinder(Vect(500,0,0), 430, 230, 50, Color(0.5, 0.5, 0.3), Material(0.01, 0.015, 0.1, 1.0, 0.9, 0.0));

//Polygons
list<Vect> ll1,ll2,lb1,lb2,lb3,lb4,lb5,lb6;

Poly pg1,pg2,pb1,pb2,pb3,pb4,pb5,pb6 ;

//lightSources
Illumination l1 = Illumination(Vect(200, 780, 100), 50);
Illumination l2 = Illumination(Vect (700, 780, 100), 50);

list<Sphere> listOfSpheres;
list<Plane> listOfPlanes;
list<Poly> listOfPolygons;
list<Illumination> listOfLight;
list<Cylinder> listOfCylinders;

double AmbInt = 5;
int windowHeight=500, windowWidth=500;

int modWindowHeight = antiAliasingFactor*windowHeight;
int modWindowWidth = antiAliasingFactor*windowWidth;

double findintersection(Ray r)
{
	Sphere closestSphere;
	Plane closestPlane;
	Poly closestPolygon;
	Cylinder closestCylinder;
	double ans = 100000;
	double minTSphere = 100000;
	double minTPlane = 100000;
	double minTPolygon = 100000;
	double minTCylinder = 100000;
	for (Sphere s : listOfSpheres) 
	{
		double t = s.getT(r);
		if (t > 0 && t < minTSphere) 
		{
			closestSphere = s;
			minTSphere = t;
		}
	}
	if (ans > minTSphere) ans = minTSphere;
	for (Plane p : listOfPlanes) 
	{
		double t = p.getT(r);
		if (t > 0 && t < minTPlane) 
		{
			closestPlane = p;
			minTPlane = t;
		}
	}	
	if (ans > minTPlane) ans = minTPlane;
	for (Poly p : listOfPolygons)
	{
	double t = p.getT(r);
	if (t > 0 && t < minTPolygon) 
	{
	closestPolygon = p;
	minTPolygon = t;
		}
	}
	if (ans > minTPolygon) ans = minTPolygon;

	for (Cylinder p : listOfCylinders) 
	{
		double t = p.getT(r);
		if (t > 0 && t < minTCylinder) 
		{
			closestCylinder = p;
			minTCylinder = t;
		}
	}
	if (ans > minTCylinder) ans = minTCylinder;

	return ans;
}

void setPixel(int x, int y, Color c) 
{
	if (c.getR() > 1.0)	c.setR(1.0);
	if (c.getG() > 1.0)	c.setG(1.0);
	if (c.getB() > 1.0)	c.setB(1.0);
	glColor3f((GLfloat)c.getR(), (GLfloat)c.getG(), (GLfloat)c.getB());
	//cout << '('<<x <<','<< y<<')'<<endl;
	//glPointSize(4.0f);
	glBegin(GL_POINTS);
	glVertex2i(x, y);
	glEnd();
	glFlush();
}

Color findColor(Ray r, int noOfLevels) 
{
	if (noOfLevels == 0)
		return Color(0,0,0);

	Sphere closestSphere;
	Plane closestPlane;
	Poly closestPolygon;
	Cylinder closestCylinder;

	double minTSphere = 100000;
	double minTPlane = 100000;
	double minTPolygon = 100000;
	double minTCylinder = 100000;

	double intens = 0;
	for (Sphere s : listOfSpheres) 
	{
		double t = s.getT(r);
		if (t > 0 && t < minTSphere) 
		{
			closestSphere = s;
			minTSphere = t;
		}
	}
	for (Plane p : listOfPlanes) 
	{
		double t = p.getT(r);

		if (t > 0 && t < minTPlane) 
		{
			closestPlane = p;
			minTPlane = t;
		}
	}
	//cout << minTSphere << endl;
	for (Poly p : listOfPolygons) 
	{
		double t = p.getT(r);
		if (t > 0 && t < minTPolygon) 
		{
			closestPolygon = p;
			minTPolygon = t;
		}
	}
	for (Cylinder p : listOfCylinders) 
	{
		double t = p.getT(r);
		if (t > 0 && t < minTCylinder) 
		{
			closestCylinder = p;
			minTCylinder = t;
		}
	}

	if (minTSphere < 100010 && minTSphere>99990 && minTPlane < 100010 && minTPlane>99990 && minTPolygon < 100010 && minTPolygon>99990 && (minTCylinder < 100010 && minTCylinder>99990))
	{ 
		return Color(0,0,0); 
	}
	if (minTSphere < minTPlane && minTSphere < minTPolygon && minTSphere<minTCylinder) 
	{
		Vect v = closestSphere.getIntersect(r);

		//Basic Illumination
		for (Illumination l : listOfLight) 
		{
			Ray shad = Ray(v, l.getPos().minus(v), r.getIntense(), r.getRefractive());
			double ans = findintersection(shad);
			double maxv = l.getPos().minus(v).getX() / shad.getDir().getX();
			//if (ans > maxv)
			intens += l.getIntensityAtIntersectSphere(closestSphere, v, eye);
		}

		double ambient = closestSphere.getMaterial().getAmbCoff()* AmbInt;
		double intensity = intens+ambient;
		Color col = closestSphere.getColor().scalarmult(intensity);

		//Reflection
		if (closestSphere.getMaterial().getCoeffForReflect() > 0.00001) 
		{
			Vect s = v;
			Vect dir = r.getDir().minus((closestSphere.getNormal(v)).scalarMult(2*((r.getDir()).dot(closestSphere.getNormal(v))))).unitVector();
			Ray reflectedRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(reflectedRay, noOfLevels - 1);
			/*if (rgb.equals(Color(0, 0, 0))) 
			{ 
				intensity += 0;
			}
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestSphere.getMaterial().getCoeffForReflect());
			}*/
			//cout << rgb.getR() << " " << rgb.getG() << " " << rgb.getB() << endl;
			//cout << v.getX() << " " << v.getY() << " " << v.getZ();
			rgb = rgb.scalarmult(closestSphere.getMaterial().getCoeffForReflect());
			//cout << rgb.getR() << " " << rgb.getG() << " " << rgb.getB() << endl;
			col.setB(col.getB()+rgb.getB());
			col.setR(col.getR() + rgb.getR());
			col.setR(col.getR() + rgb.getR());
		}

		//Refraction
		/*if (closestSphere.getMaterial().getCoeffForTransmit() > 0.00001) 
		{
			Vect s = v;
			Vect dir = closestSphere.getTransmitRayDirection(r, v);
			Ray transmitRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(transmitRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
			{ 
				intensity += 0; 
			}
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestSphere.getMaterial().getCoeffForTransmit())*(closestSphere.getMaterial().getCoeffForTransmit());
			}
		}*/

		//Color col = closestSphere.getColor().scalarmult(intensity);
		return col;
	}

	else if (minTPlane < minTSphere && minTPlane < minTPolygon && minTPlane<minTCylinder) 
	{
		Vect v = closestPlane.getIntersect(r);

		for (Illumination l : listOfLight) 
		{
			//bool shadow = false;
			Ray shad = Ray(v, l.getPos().minus(v), r.getIntense(), r.getRefractive());
			double ans = findintersection(shad);
			double maxv = l.getPos().minus(v).getX() / shad.getDir().getX();
			if (ans > maxv )
				intens += l.getIntensityAtIntersectPlane(closestPlane, v, eye);
		}

		double ambient = closestPlane.getMaterial().getAmbCoff()* AmbInt;
		double intensity = intens+ambient;

		/*if (closestPlane.getMaterial().getCoeffForReflect() > 0.00001) 
		{
			Vect s = v;
			Vect dir = r.getDir().minus((closestPlane.getNormal(v)).scalarMult(2*((r.getDir()).dot(closestPlane.getNormal(v))))).unitVector();
			Ray reflectedRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(reflectedRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
				intensity += 0;
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestPlane.getMaterial().getCoeffForReflect());
			}
		}	

		if (closestPlane.getMaterial().getCoeffForTransmit() > 0.00001) 
		{
			Vect s = v;
			Vect dir = closestPlane.getTransmitRayDirection(r, v);
			Ray transmitRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(transmitRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
			{
				intensity += 0; 
			}
			else
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestPlane.getMaterial().getCoeffForTransmit());
			}
		}*/

		Color col = closestPlane.getColor().scalarmult(intensity);
		return col;
	}

	else if (minTPolygon < minTSphere && minTPolygon < minTPlane && minTPolygon<minTCylinder) 
	{
		Vect v = closestPolygon.getIntersect(r);

		for (Illumination l : listOfLight) 
		{
			//bool shadow = false;
			Ray shad = Ray(v, l.getPos().minus(v), r.getIntense(), r.getRefractive());
			double ans = findintersection(shad);
			double maxv = l.getPos().minus(v).getX() / shad.getDir().getX();
			if (ans > maxv)
				intens += l.getIntensityAtIntersectPolygon(closestPolygon, v, eye);
		}

		double ambient = closestPolygon.getMaterial().getAmbCoff()* AmbInt;
		double intensity = ambient + intens;

		/*if (closestPolygon.getMaterial().getCoeffForReflect() > 0.00001) 
		{
			Vect s = v;
			Vect dir = r.getDir().minus((closestPolygon.getNormal(v)).scalarMult(2 *((r.getDir()).dot(closestPolygon.getNormal(v))))).unitVector();
			Ray reflectedRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(reflectedRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
				intensity += 0;
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestPolygon.getMaterial().getCoeffForReflect());
			}
		}

		if (closestPolygon.getMaterial().getCoeffForTransmit() > 0.00001) 
		{
			Vect s = v;
			Vect dir = closestPolygon.getTransmitRayDirection(r, v);
			Ray transmitRay = Ray(s, dir, 1,r.getRefractive());
			Color rgb = findColor(transmitRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
			{
				intensity += 0; 
			}
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestPolygon.getMaterial().getCoeffForTransmit());
			}
		}*/

		Color col = closestPolygon.getColor().scalarmult(intensity);
		return col;
	}

	else if (minTCylinder < minTSphere && minTCylinder < minTPolygon && minTCylinder<minTPlane) 
	{
		Vect v = closestCylinder.getIntersect(r);

		if (v.getX() != -1 && v.getY() != -1 && v.getZ() != -1)
		{
			for (Illumination l : listOfLight) 
			{
				//bool shadow = false;
				Ray shad = Ray(v, l.getPos().minus(v), r.getIntense(), r.getRefractive());
				double ans = findintersection(shad);
				double maxv = l.getPos().minus(v).getX() / shad.getDir().getX();
				if (ans > maxv)
					intens += l.getIntensityAtIntersectCylinder(closestCylinder, v, eye);
			}
		}

		double ambient = closestCylinder.getMaterial().getAmbCoff()* AmbInt;
		double intensity = intens + ambient;
		Color col = closestCylinder.getColor().scalarmult(intensity);

		//Reflection
		if (closestCylinder.getMaterial().getCoeffForReflect() > 0.00001)
		{
			Vect s = v;
			Vect dir = r.getDir().minus((closestCylinder.getNormal(v)).scalarMult(2 * ((r.getDir()).dot(closestCylinder.getNormal(v))))).unitVector();
			Ray reflectedRay = Ray(s, dir, 1, r.getRefractive());
			Color rgb = findColor(reflectedRay, noOfLevels - 1);
			/*if (rgb.equals(Color(0, 0, 0)))
			{
			intensity += 0;
			}
			else
			{
			intensity += (rgb.getIntensityFromOriginalColor())*(closestSphere.getMaterial().getCoeffForReflect());
			}*/
			//cout << rgb.getR() << " " << rgb.getG() << " " << rgb.getB() << endl;
			//cout << v.getX() << " " << v.getY() << " " << v.getZ();
			rgb = rgb.scalarmult(closestSphere.getMaterial().getCoeffForReflect());
			//cout << rgb.getR() << " " << rgb.getG() << " " << rgb.getB() << endl;
			col.setB(col.getB() + rgb.getB());
			col.setR(col.getR() + rgb.getR());
			col.setR(col.getR() + rgb.getR());
		}
/*
		if (closestPlane.getMaterial().getCoeffForTransmit() > 0.00001) 
		{
			Vect s = v;
			Vect dir = closestPlane.getTransmitRayDirection(r, v);
			Ray transmitRay = Ray(s, dir, 1, r.getRefractive());
			Color rgb = findColor(transmitRay, noOfLevels - 1);
			if (rgb.equals(Color(0, 0, 0))) 
			{	
				intensity += 0;
			}
			else 
			{
				intensity += (rgb.getIntensityFromOriginalColor())*(closestPlane.getMaterial().getCoeffForTransmit());
			}
		}*/
		return col;
	}
}



void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT| GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0, 0.0, 0.0, 0.0);//clear black	
	glColor3f(1.0, 0.0, 0.0);

	/*ll1.push_back(Vect(150, 300, -50));
	ll1.push_back(Vect(150, 300, 50));
	ll1.push_back(Vect(100, 300, 100));
	ll1.push_back(Vect(50, 300, 50));
	ll1.push_back(Vect(50, 300, -50));
	pg1 = Poly(ll1, Color(0, 1, 0), Material(0.01, 0.01, 0.1, 1,0,0));*/

	ll2.push_back(Vect(50, 230, 50));
	ll2.push_back(Vect(50, 330, 50));
	ll2.push_back(Vect(100, 380, 50));
	ll2.push_back(Vect(150, 330, 50));
	ll2.push_back(Vect(150, 230, 50));
	pg2 = Poly(ll2, Color(0, 0.4, 0.7), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb1.push_back(Vect(350, 330, -50));
	lb1.push_back(Vect(250, 330, -50));
	lb1.push_back(Vect(250, 430, -50));
	lb1.push_back(Vect(350, 430, -50));
	pb1 = Poly(lb1, Color(0.0, 1.0, 1.0), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb2.push_back(Vect(350, 330, -50));
	lb2.push_back(Vect(300, 330, 50));
	lb2.push_back(Vect(200, 330, 50));
	lb2.push_back(Vect(250, 330, -50));
	pb2 = Poly(lb2, Color(0, 1, 1), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb3.push_back(Vect(250, 430, -50));
	lb3.push_back(Vect(250, 330, -50));
	lb3.push_back(Vect(200, 330, 50));
	lb3.push_back(Vect(200, 430, 50));
	pb3 = Poly(lb3, Color(0, 1, 1), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb4.push_back(Vect(350, 430, -50));
	lb4.push_back(Vect(350, 330, -50));
	lb4.push_back(Vect(300, 330, 50));
	lb4.push_back(Vect(300, 430, 50));
	pb4 = Poly(lb4, Color(0, 1, 1), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb5.push_back(Vect(350, 430, -50));
	lb5.push_back(Vect(300, 430, 50));
	lb5.push_back(Vect(200, 430, 50));
	lb5.push_back(Vect(250, 430, -50));
	pb5 = Poly(lb5, Color(0, 1, 1), Material(0.01, 0.01, 0.1, 1, 0, 0));

	lb6.push_back(Vect(300, 330, 50));
	lb6.push_back(Vect(200, 330, 50));
	lb6.push_back(Vect(200, 430, 50));
	lb6.push_back(Vect(300, 430, 50));
	pb6 = Poly(lb6, Color(0, 1, 1), Material(0.01, 0.01, 0.9, 1.5, 0, 0));

	//listOfSpheres.push_back(s1);
	listOfSpheres.push_back(s2);

	listOfPlanes.push_back(p1);
	//listOfPlanes.push_back(p2);
	listOfPlanes.push_back(p3);
	//listOfPlanes.push_back(p4);
	//listOfPlanes.push_back(p5);

	//listOfPolygons.push_back(pg1);
	listOfPolygons.push_back(pg2);
	listOfPolygons.push_back(pb1);
	listOfPolygons.push_back(pb2);
	listOfPolygons.push_back(pb3);
	listOfPolygons.push_back(pb4);
	listOfPolygons.push_back(pb5);
	listOfPolygons.push_back(pb6);

	listOfCylinders.push_back(c1);

	listOfLight.push_back(l1);
	listOfLight.push_back(l2);

	Color c[3][500];
	for (int i = 0; i < 500; i++) {
		c[2][i]=Color(0,0,0);
		c[0][i]=Color(-1,-1,-1);
		c[1][i]=Color(-1,-1,-1);
	}
	c[1][0] = Color(0, 0, 0);
	c[1][499]= Color(0, 0, 0);
	c[0][499] = Color(0, 0, 0);
	c[0][0] = Color(0, 0, 0);
	for (int i = 1; i < windowWidth; i++) {
		for (int j = 1; j < windowHeight; j++) {
			Color fin=Color(0,0,0);
			/*for (int x=i-1; x < i+2; x++)
			{
				for (int y = j + 1; y >=j-1; y--)
				{
					if (c[j+1-y][x].equals(Color(-1,-1,-1)))
					{*/
						Vect pointOnScreen = Vect(i - windowWidth / 2, j - windowHeight / 2, 0);
						Vect pointOnScreenWCS = vcs.VCS2WCS(pointOnScreen);
						Vect dir = pointOnScreenWCS.minus(eye).unitVector();
						Ray r = Ray(eye, dir, 1, 1);
						Color finalColor = findColor(r, levelOfTracing);
						fin.setR(fin.getR() + finalColor.getR());
						fin.setB(fin.getB() + finalColor.getB());
						fin.setG(fin.getG() + finalColor.getG());
					/*}
					//else
					//{*/
					//	Color finalColor = c[j + 1 - y][x];
					//	fin.setR(fin.getR() + finalColor.getR());
					//	fin.setB(fin.getB() + finalColor.getB());
					//	fin.setG(fin.getG() + finalColor.getG());
					///*}
				/*}
			}*/
			/*fin.setR(fin.getR() / 9.0);
			fin.setB(fin.getB() / 9.0);
			fin.setG(fin.getG() / 9.0);*/
			if (fin.equals(Color(0, 0, 0))) continue;
			//cout << "in Progress " <<i<<" "<<j<< endl;
			setPixel(i, j, fin);
		}
		/*for (int x = 1; x < 499; x++)
		{
			c[2][x] = c[1][x];
			c[1][x] = c[0][x];
			c[0][x] = Color(-1, -1, -1);
		}*/
	}
	
	glutSwapBuffers();
}

void reshape(int w, int h)
{

	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, (GLdouble)w, 0.0, (GLdouble)h);
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(0, 0);
	glutInitWindowSize(windowWidth, windowHeight); 
	glutCreateWindow("OpenGL First Window");
	glewInit();
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-1.0, 1.0, -1.0, 1.0);
	glEnable(GL_DEPTH_TEST);

	
	// register callbacks
	glutDisplayFunc(renderScene);
	glutReshapeFunc(reshape);
	glutMainLoop();

	return 0;
}