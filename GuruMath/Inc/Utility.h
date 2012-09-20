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


class CVector3
{
public:
    //
    float x, y, z;

    CVector3();

    CVector3(float ix, float iy, float iz = 0.0f);

	CVector3(const CVector3& vec);

	CVector3 operator=(const CVector3& vec);

    ~CVector3();
	
    //This will treat the point as a Vector and return another vector.
    //somewhat non-mathematical, but useful for when you really want to
    // treat a point as a position vector.

    void operator+=( const CVector3& vector );
	bool operator==(const CVector3& v);

	void set(const CVector3& vec);
	void set(float ix, float iy, float iz = 0.0f );

    void normalize();
    CVector3 normal() const;

	float magnitude() const;

	/////////////////////////////////////////////////////////////////////////

    static CVector3 Cross( const CVector3& v1, const CVector3& v2 );
	static float Dot( const CVector3& v1, const CVector3& v2);
	static float AngleDot( const CVector3& v1, const CVector3& v2 );
	
	static CVector3 WorldX;
	static CVector3 WorldY;
	static CVector3 WorldZ;
};
typedef CVector3 Vector3;

CVector3 operator*(float scalar, const CVector3& vec);
CVector3 operator*(const CVector3& vec, float scalar);
CVector3 operator-(const CVector3& v1, const CVector3& v2);
CVector3 operator+( const CVector3& v1, const CVector3& v2 );
CVector3 VecMat3Multiply( const CVector3& vec, float matrix[9] );


const CVector3 principleAxes[3] =
{
	CVector3(1,0,0), CVector3(0,1,0), CVector3(0,0,-1)
};



#endif // MATH_H_INCLUDED
