/*
Author:		Dan Strycharske
Date:		09/21/09
File:		Utility.h
Brief:		A class containing many helpful math classes such as point, vector, and ray
*/

#ifndef UTILITY_H_INCLUDED
#define UTILITY_H_INCLUDED


#define _USE_MATH_DEFINES 	//To expose pi and other "non-standard c/c++" values.


#include <vector>
#include <math.h>			//Trig functions and other good math stuff.


class CVector3;
class CPoint3;
class CRay;
class Point;


class CVector3
{
public:
    //
    float x, y, z;

    CVector3() { x = 0.f; y = 0.f; z = 0.f;}

    CVector3(float ix, float iy, float iz = 0.0f)
    {
        x = ix;
        y = iy;
        z = iz;
    }

    ~CVector3() {}

	operator Point(); 

    CVector3 operator-( void );
    CVector3 operator-( const CVector3& vector );

    //This will treat the point as a Vector and return another vector.
    //somewhat non-mathematical, but useful for when you really want to
    // treat a point as a position vector.

    CVector3 operator-( const CPoint3& point );

    CVector3 operator+( const CVector3& vector );
    void operator+=( const CVector3& vector );

    friend CVector3 operator*( const CVector3& vec, float scale );

    CVector3 operator*=( float scale );

	bool operator==(const CVector3& v)
	{
		return (v.x == x && v.y == y && v.z == z) ? true : false;
	}

	void set(float ix, float iy, float iz = 0.0f )
	{
		x = ix;
		y = iy;
		z = iz;
	}

    void normalize()
    {
        float root = sqrt( x*x + y*y + z*z );
        x /= root;
        y /= root;
        z /= root;
    }

    CVector3 normal() const
    {
        float m = this->magnitude();
        return CVector3( x/m, y/m, z/m );
    }

    static CVector3 Cross( const CVector3& v1, const CVector3& v2 )
	{
		float x = v1.y * v2.z - v1.z * v2.y;
		float y = -(v1.x * v2.z - v1.z * v2.x);
		float z = v1.x * v2.y - v1.y * v2.x;

		return CVector3(x,y,z);
	}
	static float Dot( const CVector3& v1, const CVector3& v2)
	{
		return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
	}

	static float AngleDot( const CVector3& v1, const CVector3& v2 )
	{
		float dot = Dot( v1, v2 );
		float normal = v1.magnitude() * v2.magnitude();
		dot /= normal;

		return acos( dot );
	}

	float magnitude() const
	{
		return (float)sqrt( x*x + y*y + z*z );
	}
};


inline CVector3 operator*(const float& scalar, const CVector3& vec)
{
	return CVector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
}


const CVector3 principleAxes[3] =
{
	CVector3(1,0,0), CVector3(0,1,0), CVector3(0,0,-1)
};


class CPoint3
{
public:
    float x, y, z;

    CPoint3() { x = 0.f; y =0.f; z = 0.f; }
    CPoint3(float inx, float iny, float inz) : x(inx), y(iny), z(inz) {}
    ~CPoint3() {}

    CPoint3 operator-( const CVector3& vector );
    CVector3 operator-( const CPoint3& point );
    CPoint3 operator+( const CVector3& vector );
    CPoint3 operator=( const CVector3& vector );
};


class CRay
{
public:
    CRay() : dir(), origin() {}
    CRay(CVector3 d, CPoint3 p) { dir = d; origin = p; }
    CRay(float dirx, float diry, float dirz, float orix, float oriy, float oriz)
    {
        dir = CVector3(dirx, diry, dirz );
        origin = CPoint3(orix, oriy, oriz);
    }
    ~CRay() {}

    CVector3 dir;
    CPoint3 origin;
};


CVector3 VecMat3Multiply( const CVector3& vec, float matrix[9] );


#endif // MATH_H_INCLUDED
