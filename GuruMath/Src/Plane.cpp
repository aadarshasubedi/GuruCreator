/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Plane.cpp
Brief:      File containing the implementation details for Plane.
*/

#include "Primitives.h"


Plane::Plane() : mA(), mNormal() {}


Plane::Plane(const CVector3& a, const CVector3& norm) : mA(a), mNormal(norm) {}


Plane::~Plane() {}


inline
int Plane::determinePointOrientation(const CVector3& pt) //This function return -1 for below, 0 for on, 1 for above plane.
{
	//basically just project the point, and the sign of the dot product rules all.  In this case the dot product will be the distance.
	int dot = (int)projectPoint(pt);

	return (int)(dot / abs(dot));  //Should return -1, 0, or 1 depending on the side.
}


inline
float Plane::projectPoint(const CVector3& pt) //projects the vector pt - A onto the normal.
{
	CVector3 toPlane = pt - mA;
	return CVector3::Dot(normal(), toPlane);
}

