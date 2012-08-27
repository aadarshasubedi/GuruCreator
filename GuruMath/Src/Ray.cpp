/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Ray.cpp
Brief:      File containing the implementation details for Ray.
*/
#include "Primitives.h"


Ray::Ray() : mOrigin(), mDir() {}


Ray::Ray(const Point& pt, const CVector3& vec ) : mOrigin(pt), mDir(vec) {}


Ray::~Ray() {}


//Operators
bool Ray::operator==(const Point& point) //point is origin
{
	return (mOrigin.x() == point.x() &&
			mOrigin.y() == point.y() &&
			mOrigin.z() == point.z() );
}


bool Ray::operator<(const Point& point) //point in front of origin, along mDir
{
	return !determineBehind(point);
}


bool Ray::operator>(const Point& point) //point behind origin, along -mDir
{
	return determineBehind(point);
}


float Ray::operator*(const Point& point) //project distance of (point - origin) * mDir
{
	return projectPoint(point);
}


bool Ray::operator==(const Ray& )
{
	//@TODO: implement
	return false;
}


bool Ray::operator^(const Ray& )  //is vector perpendicular
{
	//@TODO: implement
	return false;
}


bool Ray::operator<(const Ray& )  //is vector pointing away, not parallel
{
	//@TODO: implement
	return false;
}


bool Ray::operator>(const Ray& )  //is vector point same way, no parallel
{
	//@TODO: implement
	return false;
}


bool Ray::operator==(const CVector3& )
{
	//@TODO: implement
	return false;
}


bool Ray::operator^(const CVector3& )  //is vector perpendicular
{
	//@TODO: implement
	return false;
}


bool Ray::operator<(const CVector3& )  //is vector pointing away, not parallel
{
	//@TODO: implement
	return false;
}


bool Ray::operator>(const CVector3& )  //is vector point same way, no parallel
{
	//@TODO: implement
	return false;
}


//English friendly versions.
bool Ray::pointIsOrigin(const Point& point)
{
	return (*this == point);
}


bool Ray::pointBehindOrigin(const Point& point)
{
	return (*this < point);
}


bool Ray::pointInFrontOfOrigin(const Point& point)
{
	return (*this > point);
}


float Ray::pointProjectedDistance(const Point& point)
{
	return (*this * point);
}


bool Ray::vectorParallel(const Vector3& )
{
	//@TODO: FInish these with helpers, and figure out why the operators won't work, it's a conversion issue between Point, CVector3, and Ray.
	return false;
}


bool Ray::vectorPerpendicular(const Vector3& )
{
	//@TODO: See 'Parallel' above.
	return false;
}


bool Ray::rayIsSame(const Ray& ray)
{
	return (*this == ray);
}


//Uses the direction vector to generate another point at origin + mDir
Line Ray::makeLine()
{
	return Line(mOrigin, mOrigin + mDir);
}


float Ray::projectPoint(const Point& point)
{
	//Vector from point - mOrigin.
	//Dot product onto mDir.normal().
	CVector3 v = point - mOrigin;
	return CVector3::Dot(v, mDir.normal());
}


bool Ray::determineBehind(const Point& point)
{
	if( *this == point || projectPoint(point) >= 0.0f )
	{
		//Point is origin or in front of.
		return false;
	}

	//It has to be behind, otherwise it's a philosophical debate.
	return true;
}

