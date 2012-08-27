/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Line.cpp
Brief:      File containing the implementation details for Line.
*/

#include "Primitives.h"


//point is on line.
bool Line::operator==(const Point& )
{
	//@TODO: implement
	return false;
}


//point is on line behind A, going away from B.
bool Line::operator>(const Point& )
{
	//@TODO: implement
	return false;
}


//point is on line in front of B, going away from A
bool Line::operator<(const Point& )
{
	//@TODO: implement
	return false;
}


bool Line::operator==(const Ray& ) //is ray parallel
{
	//@TODO: implement
	return false;
}


bool Line::operator^(const Ray& ) //is ray perpendicular
{
	//@TODO: implement
	return false;
}


//barycentric coords of the point in respect to A, B.  
//Since for two points the coords u, v must satisfy u + v = 1.0, this only needs to return one, the other can be generated.
//returns 'u'
//Only valid if the point is on the line.  Otherwise the values will be undefined.
float Line::operator*(const Point& )
{
	//@TODO: implement
	return false;
}


//dir = B - A, and A is origin.
Ray Line::makeRay()
{
	return Ray(mA, mB-mA);
}
