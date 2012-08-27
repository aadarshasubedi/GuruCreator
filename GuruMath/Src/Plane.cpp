/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Plane.cpp
Brief:      File containing the implementation details for Plane.
*/

#include "Primitives.h"


Plane::Plane() : mA(), mNormal() {}


Plane::Plane(const Point& a, const CVector3& norm) : mA(a), mNormal(norm) {}


Plane::~Plane() {}


bool Plane::operator==(const Point& pt)
{
	int result = determinePointOrientation(pt);
	return (result == 0) ? true : false;
}


bool Plane::operator<(const Point& pt)
{
	int result = determinePointOrientation(pt);
	return (result == -1) ? true : false;
}


bool Plane::operator>(const Point& pt)
{
	int result = determinePointOrientation(pt);
	return (result == 1) ? true : false;
}


float Plane::operator*(const Point& pt)
{
	//Project point returns the dot product, which in this since it was generated from a normal vector is the distance.
	//  If it is below the plane, it will be negative so just get the absolute value of the distance.
	return fabs(projectPoint(pt));
}


inline
int Plane::determinePointOrientation(const Point& pt) //This function return -1 for below, 0 for on, 1 for above plane.
{
	//basically just project the point, and the sign of the dot product rules all.  In this case the dot product will be the distance.
	int dot = (int)projectPoint(pt);

	return (int)(dot / abs(dot));  //Should return -1, 0, or 1 depending on the side.
}


inline
float Plane::projectPoint(const Point& pt) //projects the vector pt - A onto the normal.
{
	CVector3 toPlane = pt - mA;
	return CVector3::Dot(normal(), toPlane);
}

