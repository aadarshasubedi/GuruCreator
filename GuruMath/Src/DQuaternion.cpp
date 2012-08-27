/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       DQuaternion.cpp
Brief:      
/************************************************************************/

#include "DQuaternion.h"
#include <math.h>


DQuaternion::DQuaternion()
{
	x = y = z = 0.0f;
	w = 1.0f;
}


DQuaternion::DQuaternion(float r, float a, float b, float c )
{
    x = a;
    y = b;
    z = c;
    w = r;
}

DQuaternion::DQuaternion(const DQuaternion& q)
{
	*this = q;
}


DQuaternion::DQuaternion( const CVector3& vec ) // vector representation
{
	w = 1.0f;
	x = vec.x;
	y = vec.y;
	z = vec.z;
}


DQuaternion::DQuaternion( const CVector3& vec, float angle )
{
	FromAxisAngle( vec, angle );
}


DQuaternion::DQuaternion(const Matrix<>& mat) //construct from matrix.
{
}


void DQuaternion::FromAxisAngle( const CVector3& vec, float angle )
{
	//Convert to radians first.
	float radians = float(angle * (M_PI / 180.0f));

	//Cache the value of sin( radians/2);
	float sinHalfTheta = sin( radians / 2.0f);

	//Calcule w.
	w = cos( radians / 2.0f );

	//Calculate other values.
	x = float( vec.x * sinHalfTheta );
	y = float( vec.y * sinHalfTheta );
	z = float( vec.z * sinHalfTheta );
}


void DQuaternion::operator=(const DQuaternion& q)
{
	x = q.x; y = q.y; z = q.z; w = q.w;
}


DQuaternion DQuaternion::operator*( const DQuaternion& q )
{
	DQuaternion r;

	r.w = w*q.w - x*q.x - y*q.y - z*q.z;
	r.x = w*q.x + x*q.w + y*q.z - z*q.y;
	r.y = w*q.y + y*q.w + z*q.x - x*q.z;
	r.z = w*q.z + z*q.w + x*q.y - y*q.x;

	return r;
}


DQuaternion DQuaternion::operator+(const DQuaternion& q)
{
	return DQuaternion(w + q.w, x + q.x, y + q.y, z + q.z);
}


DQuaternion operator*(const DQuaternion& quat, const float& s)
{
	return DQuaternion( quat.x*s, quat.y*s, quat.z*s, quat.w*s );
}


DQuaternion operator*(const float& s, const DQuaternion& quat)
{
	return DQuaternion( quat.x*s, quat.y*s, quat.z*s, quat.w*s );
}

//@TODO:
CVector3 operator*(DQuaternion quat, CVector3 vec)
{
	DQuaternion vecQuat(vec);

	//Now just multiply the quaternions.
	DQuaternion out = quat * (vecQuat * quat.inverse());

	return CVector3(out.x, out.y, out.z);
}

//@TODO:
CVector3 operator*(CVector3 vec, DQuaternion quat)
{
	DQuaternion vecQuat(vec);

	//Now just multiply the quaternions.
	DQuaternion out = quat * (vecQuat * quat.inverse());

	return CVector3(out.x, out.y, out.z);
}



DQuaternion DQuaternion::inverse()
{
	float n = magnitude();

	DQuaternion norm(w/n, x/n, y/n, z/n);
	norm.conjugated();
	return norm;
}


void DQuaternion::inverted()
{
	//First, normalize.  Then conjugate.
	this->normalized();
	this->conjugated();
}


DQuaternion DQuaternion::normal()
{
	float n = magnitude();

	return DQuaternion(w/n, x/n, y/n, z/n);
}


void DQuaternion::normalized()
{
	float n = magnitude();
	w /= n;
	x /= n;
	y /= n;
	z /= n;
}


DQuaternion DQuaternion::conjugate()
{
	//Easy, negate the "vector" portion.
	float nx = -x;
	float ny = -y;
	float nz = -z;
	return DQuaternion(w, nx, ny, nz);
}


void DQuaternion::conjugated()
{
	x = -x;
	y = -y;
	z = -z;
}


float DQuaternion::magnitude() const
{
	float n = w*w + x*x + y*y + z*z;
	n = sqrt(n);
	return n;
}



void DQuaternion::CreateMatrix( Matrix<>* mat)
{
	//Crazy math that converts quaternion to matrix.
	float a, b, c, d;

	a = 1.0f - 2.0f * ( y*y + z*z );
	b = 2.0f * ( x*y + z*w );
	c = 2.0f * ( x*z - y*w );
	d = 0.0f;

	(*mat)(0,0) = a;
	(*mat)(1,0) = b;
	(*mat)(2,0) = c;
	(*mat)(3,0) = d;

	a = 2.0f * ( y*x - z*w );
	b = 1.0f - 2.0f * ( x*x + z*z );
	c = 2.0f * ( y*z + x*w );

	(*mat)(0,1) = a;
	(*mat)(1,1) = b;
	(*mat)(2,1) = c;
	(*mat)(3,1) = d;

	a = 2.0f * ( z*x + y*w );
	b = 2.0f * ( z*y - x*w );
	c = 1.0f - 2.0f * ( x*x + y*y );

	(*mat)(0,2) = a;
	(*mat)(1,2) = b;
	(*mat)(2,2) = c;
	(*mat)(3,2) = d;

	(*mat)(0,3) = d;
	(*mat)(1,3) = d;
	(*mat)(2,3) = d;
	(*mat)(3,3) = d;
}


void DQuaternion::CreateMatrixGL( float* matrix)
{
	matrix[0] = 1.0f - 2.0f * ( y*y + z*z );
	matrix[1] = 2.0f * ( x*y + z*w );
	matrix[2] = 2.0f * ( x*z - y*w );
	matrix[3] = 0.f;

	matrix[4] = 2.0f * ( y*x - z*w );
	matrix[5] = 1.0f - 2.0f * ( x*x + z*z );
	matrix[6] = 2.0f * ( y*z + x*w );
	matrix[7] = 0.f;

	matrix[8] = 2.0f * ( z*x + y*w );
	matrix[9] = 2.0f * ( z*y - x*w );
	matrix[10] = 1.0f - 2.0f * ( x*x + y*y );
	matrix[11] = 0.f;

	matrix[12] = 0.f;
	matrix[13] = 0.f;
	matrix[14] = 0.f;
	matrix[15] = 1.f;
}

