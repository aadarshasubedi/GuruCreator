/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Quad.cpp
Brief:      File containing the implementation details for a Quad.
*/

#include "Primitives.h"


Quad::Quad() {}


Quad::Quad(const Point& center, const CVector3& u, const CVector3& v) 
	: Plane(center, CVector3::Cross(u,v).normal()), mU(u.normal()), mV(v.normal()), mEu(u.magnitude() / 2.0f), mEv(v.magnitude() / 2.0f) 
{

}


Quad::Quad(const Point& A, const Point& B, const Point& C) 
	: Plane(A, CVector3::Cross( B - A, C - A)), mU((B-A).normal()), mV((C-A).normal())
{
	mEu = (B - A).magnitude() / 2.0f;
	mEv = (C - A).magnitude() / 2.0f;

	//The planes point is used as the quads center, so we need to adjust it.
	setA( A + mEu * mU + mEv * mV);
}


Quad::~Quad() {}
