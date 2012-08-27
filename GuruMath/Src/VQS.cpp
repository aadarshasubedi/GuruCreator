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


vqs::vqs(const Matrix<>& mat) //construct from matrix.
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


// This function converts this vqs into a 4x4 matrix.  For graphics purposes.
Matrix<> vqs::toMatrix()  const
{
	return Matrix<>();
}


void vqs::SetVQS( const Matrix<>& matrix )
{
}



//
//D3DXVECTOR3 mutliplyQuaternionVector( const D3DXVECTOR3& vec, const D3DXQUATERNION& quat )
//{
//	D3DXQUATERNION vec_quaternion( vec.x, vec.y, vec.z, 0 ), quat_inv;
//
//	D3DXQuaternionInverse( &quat_inv, &quat );
//
//	D3DXQuaternionMultiply( &vec_quaternion, &quat_inv, &vec_quaternion);
//	D3DXQuaternionMultiply( &vec_quaternion, &vec_quaternion, &quat );
//
//	return D3DXVECTOR3( vec_quaternion.x, vec_quaternion.y, vec_quaternion.z );
//}
//
//
//vqs::vqs()
//:
//vqs_translation(0,0,0), vqs_orientation(0,0,0,1), vqs_uniformscale(1.0f)
//{
//}
//
//
//vqs::vqs(const D3DXVECTOR3 &vector, const D3DXQUATERNION &quaternion, const float &scale)
//:
//vqs_translation(vector), vqs_orientation(quaternion), vqs_uniformscale(scale)
//{
//}
//
//
///*
//This function performs the following arithmetic to compute the new vector.
//
//	newvec = orient_inv * ( vec * scale ) orient + translation
//*/
//
//D3DXVECTOR3	vqs::MultiplyVector(const D3DXVECTOR3 &vec) const
//{
//	D3DXVECTOR3 result_vec;
//
//	//Now apply proper order to the multiplications.
//	result_vec = mutliplyQuaternionVector( vec, vqs_orientation );
//	result_vec *= 1.0f;
//
//	result_vec += vqs_translation;
//
//	return result_vec;
//}
//
//
//
////OPERATOR
////
//// This operator allows multiplication between two vqs structs.
//vqs				vqs::operator*( const vqs& VQS ) const
//{
//	D3DXVECTOR3 result_vec;
//	D3DXQUATERNION	result_quaternion;
//	float			result_scalar = 1.f;
//
//	result_vec = MultiplyVector( VQS.Translation() );
//
//	D3DXQuaternionMultiply( &result_quaternion, &VQS.Orientation(), &vqs_orientation );
//
//	result_scalar = vqs_uniformscale * VQS.Scale();
//
//	return vqs( result_vec, result_quaternion, result_scalar );
//}
//
//
//vqs						vqs::operator*( const float&	pScalar )
//{
//	return vqs( vqs_translation * pScalar, vqs_orientation * pScalar, vqs_uniformscale * pScalar);
//}
//
//
//
//
//vqs						vqs::operator+( const vqs&	pVQS )
//{
//	vqs		temp;
//	temp.vqs_translation = vqs_translation + pVQS.vqs_translation;
//	temp.vqs_orientation = vqs_orientation + pVQS.vqs_orientation;
//	temp.vqs_uniformscale = vqs_uniformscale + pVQS.vqs_uniformscale;
//
//	return temp;
//}
//
//
//void		vqs::SetVQS(const D3DXMATRIX &matrix)
//{
//	vqs_uniformscale = 1.0f;
//
//	//Here I set the quaternion to the combined frame matrix tranformed into a quaternion.
//	D3DXQuaternionRotationMatrix( &vqs_orientation, &matrix );
//
//	//Here I extract the translation portion out of the matrix and set the v vector to it.
//	vqs_translation = D3DXVECTOR3(matrix._41,matrix._42,matrix._43);
//}


