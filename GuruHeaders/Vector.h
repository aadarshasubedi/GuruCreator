/*
Author:		Dan Strycharske
Date:		01/31/12
File:		Vector.cpp
Brief:		Definition of the Vector template class, which is derived from Matrix.
*/

#ifndef GURU_VECTOR_H
#define GURU_VECTOR_H


#include "Matrix.h"


template <typename T=float>
class Vector : public Matrix<T>
{
public:
	Vector(unsigned int numRows = 3);
	Vector(T* elements, unsigned int numElements);
	Vector(const Vector& vec);
	Vector(const Matrix& mat);
	virtual ~Vector();

private:
	//nothing new and private, simply the bounds of the columns and rows

public:
	//Static public functions, mainly for printing and testing to console.
	//The base Matrix class printMatrix should work for Vectors.
	static void testVector();
};
typedef Vector<> Vector3;


template <typename T>
Vector<T>::Vector(unsigned int numRows) : Matrix<T>(1, numRows)
{
}


template <typename T>
Vector<T>::Vector(T* elements, unsigned int numElements) : Matrix<T>(1, numElements) 
{
	this->updateMatrix(elements);
}


template <typename T>
Vector<T>::~Vector()
{
}


template <typename T>
Vector<T>::Vector(const Vector<T>& in) : Matrix<T>(in)
{
}


template <typename T>
Vector<T>::Vector(const Matrix<T>& mat) : Matrix<T>(mat)
{
	//if the num columns of the mat is 1, we can copy.  otherwise, no go.
	if( mColumns == 1 )
	{
	}
	else
	{
		delete [] mElements;
		*(const_cast<unsigned int*>(&mColumns)) = 0;
		*(const_cast<unsigned int*>(&mRows)) = 0;
	}
}


template <typename T>
void Vector<T>::testVector()
{
	float values[3] = {10, 20, 30};
	Vector3 vec1(values, 3);
	Vector3 vec2(values, 3);

	Vector3 vec3 = vec2 + vec1;

	Vector<> vec4(values, 2);
	
	Matrix<T>::printMatrix(vec1);
	Matrix<T>::printMatrix(vec3);

	//@TODO:  This is bad, adding two vectors who are not the same size.  
	Vector<> vec5 = vec4 + vec3;
	Matrix<T>::printMatrix(vec5);

	float values2[4] = {10,20,30,40};
	float ident[16] = { 1, 0, 0, 0,
					  0, 1, 0, 0, 
					  0, 0, 1, 0, 
					  0, 0, 0, 1 };
	
	Matrix4 mat;
	mat.updateMatrix(ident);

	Vector<> vect(values2, 4);

	Vector<> ret = mat * vect;
	Matrix<T>::printMatrix(ret);
}


#endif
