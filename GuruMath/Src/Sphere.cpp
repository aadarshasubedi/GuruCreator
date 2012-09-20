/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Sphere.cpp
Brief:      File containing the implementation details for Sphere.
*/

#include "Primitives.h"

//Constructors/Destructor
Sphere::Sphere() : mRadius(0.0f), mCenter() {}


Sphere::Sphere(const float& radius, const CVector3& center) : mRadius(radius), mCenter(center) {}


Sphere::~Sphere() {}

/*

//Operators
//POINTS
bool Sphere::operator==(const CVector3& point) //point is on sphere surface.
{	
	return (*this * point == mRadius);
}


bool Sphere::operator<(const CVector3& point ) //point is outside sphere.
{
	return (*this * point > mRadius);
}


bool Sphere::operator>(const Point& point) //point is inside sphere.
{
	return ( *this * point < mRadius);
}


//point distance to sphere center.  
//Add/Subtract radius for distance to center.
float Sphere::operator*(const Point& point) 
{
	return distPointToSphereCenter(point, *this);
}



//LINES
//both points lie on the sphere surface.
bool Sphere::operator==(const Line& line)
{
	//@TODO: implement
	float distA = distPointToSphereSurface(line.A(), *this);
	float distB = distPointToSphereSurface(line.B(), *this);

	return ( distA == 0.0f && distB == 0.0f ) ? true : false;
}


//line is fully outside of sphere.
bool Sphere::operator<(const Line& line)
{
	//if the shortest distance is negative or zero, it means the line
	// straddles the surface or intersects the sphere.
	return (*this * line > 0.0f);
}


//line is fully inside of sphere.
bool Sphere::operator>(const Line& line)
{
	float distA = distPointToSphereCenter(line.A(), *this);
	float distB = distPointToSphereCenter(line.B(), *this);

	return (distA < mRadius && distB < mRadius);
}


//shortest linear distance to the sphere surface.
//negative if fully contained.
//add/subtract radius for distance to center.
float Sphere::operator*(const Line& line ) 
{
	return distSphereToLine(*this, line);
}



//TRIANGLES
//all points lie on the sphere surface.
bool Sphere::operator==(const Triangle& triangle)
{
	float distA = distPointToSphereSurface(triangle.A(), *this);
	float distB = distPointToSphereSurface(triangle.B(), *this);
	float distC = distPointToSphereSurface(triangle.C(), *this);

	return (distA == 0.0f && distB == 0.0f && distC == 0.0f);
}


//tri is fully outside of sphere.
bool Sphere::operator<(const Triangle& triangle)
{
	return (*this * triangle > 0.0f);
}


//tri is fully inside of sphere.
bool Sphere::operator>(const Triangle& triangle)
{
	float distA = distPointToSphereCenter(triangle.A(), *this);
	float distB = distPointToSphereCenter(triangle.B(), *this);
	float distC = distPointToSphereCenter(triangle.C(), *this);

	return (distA < mRadius && distB < mRadius && distC < mRadius);
}


//shortest linear distance to the sphere surface.  
//negative if fully contained.
//add/subtract radius for distance to center.
float Sphere::operator*(const Triangle& triangle) 
{
	return distSphereToTriangle(*this, triangle);
}
*/

//
////English friendly funcs.
//bool Sphere::pointOnSurface(const Point& point)
//{
//	return (*this == point);
//}
//
//
//bool Sphere::pointOutside(const Point& point)
//{
//	return (*this < point);
//}
//
//
//bool Sphere::pointInside(const Point& point)
//{
//	return (*this > point);
//}
//
//
//float Sphere::pointDistanceToSurface(const Point& point)
//{
//	return (*this * point);
//}
//
//
//bool Sphere::lineFullyOutside(const Line& line)
//{
//	return (*this < line);
//}
//
//
//bool Sphere::lineFullyInside(const Line& line)
//{
//	return (*this > line);
//}
//
//
//float Sphere::lineDistanceToSurface(const Line& line)
//{
//	return (*this * line);
//}
//
//
//bool Sphere::triangleOnSurface(const Triangle& triangle)
//{
//	return (*this == triangle);
//}
//
//
//bool Sphere::triangleFullyOutside(const Triangle& triangle)
//{
//	return (*this < triangle);
//}
//
//
//bool Sphere::triangleFullyInside(const Triangle& triangle)
//{
//	return (*this > triangle);
//}
//
//
//float Sphere::triangleDistanceToSurface(const Triangle& triangle)
//{
//	return (*this * triangle);
//}
