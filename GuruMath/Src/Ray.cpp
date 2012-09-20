/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Ray.cpp
Brief:      File containing the implementation details for Ray.
*/
#include "Primitives.h"


Ray::Ray() : mOrigin(), mDir() {}


Ray::Ray(const CVector3& pt, const CVector3& vec ) : mOrigin(pt), mDir(vec) {}


Ray::~Ray() {}



//English friendly versions.
bool Ray::pointIsOrigin(const CVector3& point)
{
	return false;
}


bool Ray::pointBehindOrigin(const CVector3& point)
{
	return false;
}


bool Ray::pointInFrontOfOrigin(const CVector3& point)
{
	return false;
}


float Ray::pointProjectedDistance(const CVector3& point)
{
	return false;
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
	return false;
}


//Uses the direction vector to generate another point at origin + mDir
Line Ray::makeLine()
{
	return Line(mOrigin, mOrigin + mDir);
}


float Ray::projectPoint(const CVector3& point)
{
	//Vector from point - mOrigin.
	//Dot product onto mDir.normal().
	CVector3 v = point - mOrigin;
	return CVector3::Dot(v, mDir.normal());
}


bool Ray::determineBehind(const CVector3& point)
{
	if( pointIsOrigin(point) || projectPoint(point) >= 0.0f )
	{
		//Point is origin or in front of.
		return false;
	}

	//It has to be behind, otherwise it's a philosophical debate.
	return true;
}

