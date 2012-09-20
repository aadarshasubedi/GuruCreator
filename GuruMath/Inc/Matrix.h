/*
Author: Dan Strycharske
Date:   01.28.12
File:   Matrix.h
Brief:  A class defining the matrix math construct.  This can in turn be used to define vectors as well.
		Obvious templatization to this class so matrix of any type is usable.
		I'm thinking a 4x4 matrix of floats is the most commonly used, so that will be the default if just 'Matrix'
		is used with no type specifier for the template argument.
*/

#ifndef GURU_MATRIX_H
#define GURU_MATRIX_H

#include <iostream>


////
class Matrix4;
////


class Vector4
{
public:
	//Set element.
	enum AXIS
	{
		X = 0,
		Y,
		Z,
		W
	};

public:
	Vector4() : x(mElements[0]), y(mElements[1]), z(mElements[2]), w(mElements[3]) 
	{}

	Vector4(float ix, float iy, float iz, float iw) : x(mElements[0]), y(mElements[1]), z(mElements[2]), w(mElements[3])
	{
		mElements[0] = ix; 
		mElements[1] = iy; 
		mElements[2] = iz; 
		mElements[3] = iw;
	}

	~Vector4() {}

	Vector4(const Vector4& in) : x(mElements[0]), y(mElements[1]), z(mElements[2]), w(mElements[3])
	{
		*this = in;
	}

	Vector4& operator=(const Vector4& in)
	{
		if( this == &in)
			return *this;

		this->setAllElements(in.x, in.y, in.z, in.w);
		return *this;
	}
	
	inline void setAllElements(float ix, float iy, float iz, float iw)
	{
		mElements[X] = ix;
		mElements[Y] = iy;
		mElements[Z] = iz;
		mElements[W] = iw;
	}

	inline void setElement(AXIS elm, const float& value)
	{
		mElements[elm] = value;
	}

	inline float getElement(AXIS elm)
	{
		return mElements[elm];
	}

public:
	//Static functions for cross and dot.
	static float dot(const Vector4& v1, const Vector4 v2);
	static Vector4 cross(const Vector4& v1, const Vector4 v2);

	float& x,y,z,w;

private:
	float mElements[4];
};

Vector4 operator+(const Vector4& v1, const Vector4& v2);
Vector4 operator-(const Vector4& v1, const Vector4& v2);

Vector4 operator*(const float& value, const Vector4& vec);
Vector4 operator*(const Vector4& vec, const float& value);


/*

A 4x4 matrix class.

*/
class Matrix4
{
public:
	//Default constructor
	Matrix4();
	Matrix4(const float* elements);

	//Destructor
	~Matrix4();
	
	////////////////////////////////////////////////////////////////////
	//Setter from a simple array.
	////////////////////////////////////////////////////////////////////
	void setAllElements(const float* elements);

	////////////////////////////////////////////////////////////////////
	//Single value index, already calculated row and column.
	////////////////////////////////////////////////////////////////////
	void setElement(const int& index, const float& value);
	void setElement(const int& row, const int& col, const float& value);
	
	////////////////////////////////////////////////////////////////////
	//Single value index, already calculated row and column.
	////////////////////////////////////////////////////////////////////
	float getElement(const int& elm);
	float getElement(const int& row, const int& col);

	////////////////////////////////////////////////////////////////////
	////Get rows and columns of a matrix.
	Vector4 getRow(Vector4::AXIS axisRow) const;
	Vector4 getColumn(Vector4::AXIS axisRow) const;

	////////////////////////////////////////////////////////////////////
	//Indexing operators.
	////////////////////////////////////////////////////////////////////
	const float& operator[](unsigned int index) const;	
	float& operator[](unsigned int index);
	const float& operator()(unsigned int r, unsigned int c);

	////////////////////////////////////////////////////////////////////
	//Matrix-specific functions
	////////////////////////////////////////////////////////////////////
	bool getInverse(Matrix4& invOut);


	////////////////////////////////////////////////////////////////////
	//Private data elements.
	////////////////////////////////////////////////////////////////////
private:
	float mElements[16];
};

Vector4 operator*(const Matrix4& matrix, const Vector4& vec);


#endif // GURU_MATRIX_H
