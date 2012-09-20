/************************************************************************
Author:		Dan Strycharske
Date:       02.12.12
File:       VQS.cpp
Brief:      
/************************************************************************/

#include "VQS.h"

vqs::vqs() : mTranslation(), mOrientation(), mScalar(1.0f)
{
}


vqs::vqs(const vqs& inVqs) //copy constructor.
{
	*this = inVqs;
}


vqs::vqs(const CVector3& vec, float angle) : mTranslation(), mOrientation(vec, angle), mScalar(1.0f)
{
}


//Accessors and mutators
const CVector3& vqs::translation() const
{
	return mTranslation;
}


CVector3 vqs::translation()
{
	return mTranslation;
}


const DQuaternion& vqs::orientation() const
{
	return mOrientation;
}


DQuaternion vqs::orientation()
{
	return mOrientation;
}


const float& vqs::scale() const
{
	return mScalar;
}


float vqs::scale()
{
	return mScalar;
}


vqs vqs::operator*( const float&	pScalar )
{
	vqs result;
	result.Set(translation() * pScalar, orientation() * pScalar, scale() * pScalar);
	return result;
}


vqs vqs::operator+( const vqs&	pVQS )
{
	CVector3 transAdd = translation() + pVQS.translation();
	DQuaternion quatAdd = orientation() + pVQS.orientation();
	float scaleAdd = scale() + pVQS.scale(); 
	vqs result;
	result.Set(transAdd, quatAdd, scaleAdd);
	return result;
}


vqs vqs::operator*( const vqs& VQS ) const
{
	//holds the resulting vqs.
	CVector3 result_translation;
	float result_scale = 1.0f;
	DQuaternion result_orientation;

	//Apply rotation to the incoming vqs's translation
	result_translation = this->orientation() * VQS.translation();

	//Multiply the rotations to achieve final orientation.
	DQuaternion curr = this->orientation();
	DQuaternion next = VQS.orientation();
	result_orientation = next * curr;
	
	//Multiply scalers as well.
	result_scale = VQS.scale() * this->scale();

	//Set and return result.
	vqs result;
	result.Set(result_translation, result_orientation, result_scale);
	return result;
}


// This member function multiplies this vqs with the given vector3 vec.  As
//  shown in notes.
CVector3 vqs::MultiplyVector( const CVector3& vec ) const
{
	//Turn the vector into a quaternion-vector.
	DQuaternion quatVec(vec);
	
	//Get the inverse of the current quaternion.
	DQuaternion inverse = this->orientation();
	inverse.inverted();

	//Now multiply inverse*quatVec, then quatVec*normal
	quatVec = inverse * quatVec;
	quatVec = quatVec * this->orientation();

	//Easily transforms back into a vector.
	CVector3 result(quatVec.x, quatVec.y, quatVec.z);
	
	//Apply translation to vector.
	result = result + this->translation();

	//Apply scale after translation.
	result = result * this->scale();

	//operation is complete.
	return result;
}

void vqs::setFromMatrix( Matrix4 mat )
{
}

