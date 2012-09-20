/*
Author:		Dan Strycharske
Date:
File:
Brief:



*/

#include "Utility.h"
#include "Primitives.h"


CVector3 CVector3::WorldX = CVector3(1,0,0);
CVector3 CVector3::WorldY = CVector3(0,1,0);
CVector3 CVector3::WorldZ = CVector3(0,0,1);


CVector3::CVector3() { x = 0.f; y = 0.f; z = 0.f;}

CVector3::CVector3(float ix, float iy, float iz)
{
    x = ix;
    y = iy;
    z = iz;
}

CVector3::CVector3(const CVector3& vec)
{
	set(vec);
}

CVector3 CVector3::operator=(const CVector3& vec)
{
	if( this == &vec)
		return *this;

	set(vec);
	return *this;
}

CVector3::~CVector3() {}
	
//This will treat the point as a Vector and return another vector.
//somewhat non-mathematical, but useful for when you really want to
// treat a point as a position vector.

void CVector3::operator+=( const CVector3& vector )
{
	x += vector.x;
	y += vector.y;
	z += vector.z;
}

bool CVector3::operator==(const CVector3& v)
{
	return (v.x == x && v.y == y && v.z == z) ? true : false;
}

void CVector3::set(const CVector3& vec)
{
	x = vec.x;
	y = vec.y;
	z = vec.z;
}

void CVector3::set(float ix, float iy, float iz )
{
	x = ix;
	y = iy;
	z = iz;
}

void CVector3::normalize()
{
    float root = sqrt( x*x + y*y + z*z );
    x /= root;
    y /= root;
    z /= root;
}

CVector3 CVector3::normal() const
{
    float m = this->magnitude();
    return CVector3( x/m, y/m, z/m );
}

CVector3 CVector3::Cross( const CVector3& v1, const CVector3& v2 )
{
	float x = v1.y * v2.z - v1.z * v2.y;
	float y = -(v1.x * v2.z - v1.z * v2.x);
	float z = v1.x * v2.y - v1.y * v2.x;

	return CVector3(x,y,z);
}
float CVector3::Dot( const CVector3& v1, const CVector3& v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

float CVector3::AngleDot( const CVector3& v1, const CVector3& v2 )
{
	float dot = Dot( v1, v2 );
	float normal = v1.magnitude() * v2.magnitude();
	dot /= normal;

	return acos( dot );
}

float CVector3::magnitude() const
{
	return (float)sqrt( x*x + y*y + z*z );
}

CVector3 operator*(float scalar, const CVector3& vec)
{
	return CVector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

CVector3 operator*(const CVector3& vec, float scalar)
{
	return CVector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}

CVector3 operator-(const CVector3& v1, const CVector3& v2)
{
	return CVector3(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

CVector3 operator+( const CVector3& v1, const CVector3& v2 )
{
	return CVector3(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
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
