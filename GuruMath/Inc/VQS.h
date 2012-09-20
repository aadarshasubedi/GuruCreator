/*
author:	Dan Strycharske

date:	02/21/08
file:	VQS.h

brief:	This class defines the vqs structure described in class.  Implements
		the functionality I need such as vector multiplation and concatenation.
*/

#ifndef VQS_H
#define VQS_H


//********CLASS*************
//
//brief:	Vector for translation
//		Quaternion for orientation
//		Scalar for uniform-scale
//
//		The class implements multiple constructors as well
//		as functions for multiplying vectors, vqs's together,
//		and concatenation.
//
//members:
//		D3DXVECTOR3		vqs_translation;
//		D3DXQUATERNION	vqs_orientation;
//		float			vqs_uniformscale;
//
//*******/
#include "Utility.h"
#include "DQuaternion.h"


class vqs
{
private:
	CVector3 mTranslation;
	DQuaternion mOrientation;
	float mScalar;

public:
	vqs(); //identity vqs.
	vqs(const vqs& inVqs); //copy constructor.
	vqs(const CVector3& vec, float angle);  //axis-angle constructor.

	//Accessors and mutators
	const CVector3& translation() const;
	CVector3 translation();
	
	const DQuaternion& orientation() const;
	DQuaternion orientation();
	
	const float& scale() const;
	float scale();
	
	void setFromMatrix( Matrix4 mat );

	void Set(CVector3 vec, DQuaternion quat, float scale)
	{
		mTranslation = vec;
		mOrientation = quat;
		mScalar = scale;
	}

	vqs operator*( const float&	pScalar );
	vqs operator+( const vqs&	pVQS );	
	vqs operator*( const vqs& VQS ) const;
	//vqs operator*( const CVector3& vec) const;

	// This member function multiplies this vqs with the given vector3 vec.  As
	//  shown in notes.
	CVector3 MultiplyVector( const CVector3& vec ) const;
};



#endif
