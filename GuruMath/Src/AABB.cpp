/*
Author:		Dan Strycharske
Date:       02.01.12
File:       AABB.cpp
Brief:      File containing the implementation details for a AABB.
*/

#include "Primitives.h"


AABB::AABB()
{

}


AABB::AABB(const CVector3& center, float* h_extents) 
	: mCenter(center), mHalfExtents(h_extents[0], h_extents[1], h_extents[2])
{

}


AABB::AABB(const CVector3& center, float h_width, float h_height, float h_depth) 
	: mCenter(center), mHalfExtents(h_width, h_height, h_depth)  
{
}


AABB::AABB(const CVector3& minPt, const CVector3& maxPt, bool minMaxRep)
	: mCenter(minPt), mHalfExtents(maxPt)
{
	if( minMaxRep )
	{
		//Generate vector, which will be the full extents.  Half this.
		CVector3 h_extents = (maxPt - minPt) * 0.5f;

		CVector3 center = minPt + h_extents;
		setCenter(minPt);
		setExtents(h_extents);
	}
}


AABB::~AABB()
{
}


AABB AABB::convertToFullExtents()
{
	//@TODO: 1 Start here, these will be easiest.
	return AABB();
}


AABB AABB::convertToMinMaxPts()
{
	//@TODO: 2 continue here.
	
	//Take center, and go back negative half extents.
	CVector3 minPt = mCenter - extents();
	CVector3 maxPt = mCenter + extents();

	CVector3 vec = maxPt;

	setCenter(minPt); //center acts as the miniumum CVector3 in the MinMax representation.
	setExtents(maxPt); //	

	return AABB();
}


//The slab is used in the engine during detection, but it does not alter or create a new
// AABB so it is safe to use anywhere.
void AABB::convertToSlab(float* min, float* max)  //assumes allocated to 6 elements.
{
	convertToSlab(min[0], max[0], min[1], max[1], min[2], max[2]);
}

void AABB::convertToSlab(float& xmin, float& xmax, float& ymin, float& ymax, float& zmin, float& zmax)
{
	AABB minMaxRep = this->convertToMinMaxPts();
	//ONce we have that, it's just simply copying the values over.
	xmin = minMaxRep.mCenter.x;
	ymin = minMaxRep.mCenter.y;
	zmin = minMaxRep.mCenter.z;
	
	xmax = minMaxRep.mHalfExtents.x;
	ymax = minMaxRep.mHalfExtents.y;
	zmax = minMaxRep.mHalfExtents.z;
}
