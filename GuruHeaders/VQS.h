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
#include "Matrix.h"


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
	vqs(const Matrix<>& mat); //construct from matrix.

	//Accessors and mutators
	const CVector3& translation() const;
	CVector3 translation();
	
	const DQuaternion& orientation() const;
	DQuaternion orientation();
	
	const float& scale() const;
	float scale();

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
	
	// This function converts this vqs into a 4x4 matrix.  For graphics purposes.
	Matrix<> toMatrix()  const;	
	void SetVQS( const Matrix<>& matrix );
};











//class vqs
//{
//public:
//	//Default constructor, everything intialized to identity or zero.
//	vqs();
//
//	//Constructor taking
//	vqs( const D3DXVECTOR3& vector, const D3DXQUATERNION& quaternion, const float& scale );
//
//	//Accessors and mutators
//	const D3DXVECTOR3&		Translation() const;
//	D3DXVECTOR3&		Translation();
//
//	const D3DXQUATERNION&	Orientation() const;
//	D3DXQUATERNION&	Orientation();
//
//	const float&			Scale() const;
//	float&					Scale() ;
//
//	vqs						operator*( const float&	pScalar );
//	vqs						operator+( const vqs&	pVQS );
//
//	//FUNCTION
//	//
//	// This member function multiplies this vqs with the given vector3 vec.  As
//	//  shown in notes.
//	D3DXVECTOR3		MultiplyVector( const D3DXVECTOR3& vec ) const;
//
//
//
//	//OPERATOR
//	//
//	// This operator allows multiplication between two vqs structs.
//	vqs				operator*( const vqs& VQS ) const;
//
//
//	//FUNCTION
//	//
//	// This functino converts this vqs into a d3dx matrix.  For graphics purposes.
//	D3DXMATRIX		toMatrix()  const;
//
//
//	void			SetVQS( const D3DXMATRIX& matrix );
//
//private:
//	//The three members of the vqs data member.
//		D3DXVECTOR3		vqs_translation;
//		D3DXQUATERNION	vqs_orientation;
//		float			vqs_uniformscale;
//};
//
//
//
////Inline functions.  Accessors mostly.
//inline const D3DXVECTOR3& vqs::Translation() const
//{
//	return vqs_translation;
//}
//
//inline D3DXVECTOR3& vqs::Translation()
//{
//	return vqs_translation;
//}
//
//inline const D3DXQUATERNION& vqs::Orientation() const
//{
//	return vqs_orientation;
//}
//
//inline D3DXQUATERNION& vqs::Orientation()
//{
//	return vqs_orientation;
//}
//
//inline const float& vqs::Scale() const
//{
//	return vqs_uniformscale;
//}
//
//inline float& vqs::Scale()
//{
//	return vqs_uniformscale;
//}
//
//inline D3DXMATRIX vqs::toMatrix() const
//{
//	D3DXMATRIX final;
//	D3DXQUATERNION norm;
//	D3DXQuaternionNormalize( &norm, &vqs_orientation );
//	D3DXMatrixRotationQuaternion( &final, &norm );
//
//	final._11 *= vqs_uniformscale;
//	final._22 *= vqs_uniformscale;
//	final._33 *= vqs_uniformscale;
//
//	final._41 = vqs_translation.x;
//	final._42 = vqs_translation.y;
//	final._43 = vqs_translation.z;
//
//	return final;
//}





#endif
