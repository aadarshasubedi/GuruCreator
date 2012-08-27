/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Point.cpp
Brief:      File containing the implementation details for Point.
*/
#include "Primitives.h"


Point::Point(float ix, float iy, float iz) : mX(ix), mY(iy), mZ(iz) {}

Point::Point(const Point& pt) { *this = pt; }

Point::Point(const CVector3& v) : mX(v.x), mY(v.y), mZ(v.z) {}

Point::Point(CVector3 v) : mX(v.x), mY(v.y), mZ(v.z) {}

Point::~Point() {}


//This function takes a second point and forms a ray from the two points.
Ray Point::makeRay(const Point& B)
{
	CVector3 v(B.x() - mX, B.y() - mY, B.z() - mZ);
	return Ray(*this, v);
}


Point operator+(const Point& pt, const CVector3& vec)
{
	return Point(pt.x() + vec.x, pt.y() + vec.y, pt.z() + vec.z);
}


Point operator-(const Point& pt, const CVector3& vec)
{
	return pt + (vec * -1.0f);
}


bool operator==(const Point& A, const Point& B)
{
	return (A.x() == B.x() && A.y() == B.y() && A.z() == B.z());
}


CVector3 operator-(const Point& A, const Point& B)
{
	//Given as A and B, v = B - A, where A is the "origin" of the vector.
	return CVector3(A.x() - B.x(), A.y() - B.y(), A.z() - B.z());
}


Point operator*( const Matrix<>& mat)
{
	Vector<> vec;

	if( mat.numColumns() == 3 && mat.numRows() == 3)
	{

	}
	else if( mat.numColumns() == 4 && (mat.numRows() == 4 || mat.numRows() == 3) )
	{

	}
	return Point();
}
