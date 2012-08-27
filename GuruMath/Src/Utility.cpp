/*
Author:		Dan Strycharske
Date:
File:
Brief:



*/

#include "Utility.h"
#include "Primitives.h"



CVector3::operator Point()
{
	return Point(x, y, z);
}


CVector3 CVector3::operator-()
{
    return CVector3( -x, -y, -z );
}


CVector3 CVector3::operator-( const CVector3& vector )
{
	return CVector3( x - vector.x, y - vector.y, z - vector.z );
}


//This will treat the point as a Vector and return another vector.
//somewhat non-mathematical, but useful for when you really want to
// treat a point as a position vector.
CVector3 CVector3::operator-( const CPoint3& point )
{
	return CVector3( x - point.x, y - point.y, z - point.z );
}


CVector3 CVector3::operator+( const CVector3& vector )
{
	return CVector3( x + vector.x, y + vector.y, z + vector.z );
}


void CVector3::operator+=( const CVector3& rhs )
{
    *this = *this + rhs;
}


//inline CVector3 CVector3::operator*( float scale, const CVector3& vector )
//{
//	return (vector * scale);
//}


CVector3 operator*( const CVector3& vec, float scale )
{
	return CVector3(vec.x*scale, vec.y*scale, vec.z*scale);
}



CPoint3 CPoint3::operator-( const CVector3& vector )
{
	return CPoint3( x - vector.x, y - vector.y, z - vector.z );
}


CVector3 CPoint3::operator-( const CPoint3& point )
{
	return CVector3( x - point.x, y - point.y, z - point.z );
}


CPoint3 CPoint3::operator+( const CVector3& vector )
{
	return CPoint3( x + vector.x, y + vector.y, z + vector.z );
}


CPoint3 CPoint3::operator=( const CVector3& vector )
{
	x = vector.x;
	y = vector.y;
	z = vector.z;
	return *this;
}




CVector3 VecMat3Multiply( const CVector3& vec, float matrix[9] )
{
    CVector3 a(matrix[0], matrix[1], matrix[2]);
    CVector3 b(matrix[3], matrix[4], matrix[5]);
    CVector3 c(matrix[6], matrix[7], matrix[8]);

    float x = CVector3::Dot( vec, a );
    float y = CVector3::Dot( vec, b );
    float z = CVector3::Dot( vec, c );

    return CVector3( x, y, z );
}
