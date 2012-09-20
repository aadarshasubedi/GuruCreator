/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Quad.cpp
Brief:      File containing the implementation details for a Quad.
*/

#include "Primitives.h"


Quad::Quad() {}


Quad::Quad(const CVector3& center, const CVector3& u, const CVector3& v, bool pointRepresentation) 
	: Plane(center, CVector3::Cross(u,v).normal()), mU(u.normal()), mV(v.normal()), mEu(u.magnitude() / 2.0f), mEv(v.magnitude() / 2.0f) 
{
	if( pointRepresentation )
	{		
		this->mNormal = CVector3::Cross(u - center, v - center);
		this->mU = (u-center).normal();
		this->mV = (v-center).normal();
		mEu = (u - center).magnitude() / 2.0f;
		mEv = (v - center).magnitude() / 2.0f;

		//The planes point is used as the quads center, so we need to adjust it.
		setA( center + mEu * mU + mEv * mV);
	}
}

Quad::~Quad() {}
