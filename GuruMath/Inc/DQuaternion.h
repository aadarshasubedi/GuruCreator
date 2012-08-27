/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef DQUATERNION_H_INCLUDED
#define DQUATERNION_H_INCLUDED


#include "Utility.h"
#include "Matrix.h"


class DQuaternion
{
public:
	float x, y, z, w;

public:
	DQuaternion(); //identity/default quaternion.
	DQuaternion(float r, float a, float b, float c ); //pure complex representation
	DQuaternion( const CVector3& vec ); // vector representation
	DQuaternion( const CVector3& vec, float angle ); //vector-angle representation
	DQuaternion( const DQuaternion& q); //copy constructor
	DQuaternion(const Matrix<>& mat); //construct from matrix.

	void FromAxisAngle( const CVector3& vec, float angle );
	DQuaternion operator*(const DQuaternion& q);
	DQuaternion operator+(const DQuaternion& q);
	void operator=(const DQuaternion& q);

	DQuaternion inverse();
	void inverted();

	DQuaternion normal();
	void normalized();

	DQuaternion conjugate();
	void conjugated();

	float magnitude() const;

	void CreateMatrix(Matrix<>* mat);
	void CreateMatrixGL(float* matrix);
};


DQuaternion operator*(DQuaternion quat, float s);
DQuaternion operator*(float s, DQuaternion quat);

CVector3 operator*(DQuaternion quat, CVector3 vec);
CVector3 operator*(CVector3 vec, DQuaternion quat);


#endif // DQUATERNION_H_INCLUDED
