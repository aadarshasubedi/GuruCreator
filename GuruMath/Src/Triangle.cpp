/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Triangle.cpp
Brief:      File containing the implementation details for Triangle.
*/
#include "Primitives.h"


Triangle::Triangle() {}


Triangle::Triangle(const Point& a, const Point& b, const Point& c) : mA(a), mB(b), mC(c) {}


Triangle::~Triangle() {}



bool Triangle::operator==(const Point& point)
{
	//@TODO: implement
	return (&point) ? false : true;
}


bool Triangle::operator>(const Point& point)
{
	//@TODO: implement
	return (&point) ? false : true;
}


bool Triangle::operator<(const Point& point)
{
	//@TODO: implement
	return (&point) ? false : true;
}


float Triangle::operator*(const Point& point)
{
	//@TODO: implement
	return distPointToTriangle(point, *this);
}



//Ray
bool Triangle::operator==(const Ray& ray)
{
	//@TODO: implement
	return (&ray) ? false : true;
}


bool Triangle::operator^(const Ray& ray)
{
	//@TODO: implement
	return (&ray) ? false : true;
}


bool Triangle::operator|(const Ray& ray)
{
	//@TODO: implement
	return (&ray) ? false : true;
}


float Triangle::operator*(const Ray& ray)
{
	//@TODO: implement
	return distRayToTriangle(ray, *this);
}



//Line
bool Triangle::operator==(const Line& line)
{
	//@TODO: implement
	return (&line) ? false : true;
}


bool Triangle::operator>(const Line& line)
{
	//@TODO: implement
	return (&line) ? false : true;
}


bool Triangle::operator<(const Line& line)
{
	//@TODO: implement
	return (&line) ? false : true;
}


float Triangle::operator*(const Line& line)
{
	//@TODO: implement
	return (&line) ? false : true;
}



//Plane
bool Triangle::operator==(const Plane& plane)
{
	//@TODO: implement
	return (&plane) ? false : true;
}


//bool Triangle::operator^(const Plane& plane)
//{
//	//@TODO: implement
//	return (&plane) ? false : true;
//}


//bool Triangle::operator|(const Plane& plane)
//{
//	//@TODO: implement
//	return (&plane) ? false : true;
//}


float Triangle::operator*(const Plane& plane)
{
	//@TODO: implement
	return distPlaneToTriangle(plane, *this);
}


//Get a plane from the triangle.  Use A as the point on the plane.
Plane Triangle::getPlane()
{
	return Plane(mA, getNormal());
}


//This function takes 3 Line objects that it will fill in with the appropriate points.
void Triangle::formLines(Line& one, Line& two, Line& three)
{
	//A, B
	one.A(mA); one.B(mB);

	//B, C
	two.A(mB); two.B(mC);

	//C, A
	three.A(mC); three.B(mA);
}

CVector3 Triangle::getNormal() const
{
	CVector3 u = mB - mA;
	CVector3 v = mC - mA;

	return CVector3::Cross(u,v).normal();
}
