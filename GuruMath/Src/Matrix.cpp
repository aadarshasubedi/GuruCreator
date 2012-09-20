
#include "Matrix.h"


//Static functions for cross and dot.
float Vector4::dot(const Vector4& v1, const Vector4 v2)
{
	return (v1.x*v2.x + v1.y*v2.y + v1.z*v2.z);
}

Vector4 Vector4::cross(const Vector4& v1, const Vector4 v2)
{
	return v1;
}




Matrix4::Matrix4() {}

Matrix4::Matrix4(const float* elements)
{
	setAllElements(elements);
}

//Destructor
Matrix4::~Matrix4() {}
	
//Setter from a simple array.
void Matrix4::setAllElements(const float* elements)
{
	for( unsigned int i = 0; i < 16; ++i )
	{
		mElements[i] = elements[i];
	}
}

////////////////////////////////////////////////////////////////////
//Single value index, already calculated row and column.
////////////////////////////////////////////////////////////////////
void Matrix4::setElement(const int& index, const float& value)
{
	mElements[index] = value;
}
void Matrix4::setElement(const int& row, const int& col, const float& value)
{
	mElements[row*4 + col] = value;
}
////////////////////////////////////////////////////////////////////
	
////////////////////////////////////////////////////////////////////
//Single value index, already calculated row and column.
////////////////////////////////////////////////////////////////////
float Matrix4::getElement(const int& elm)
{
	return mElements[elm];
}
float Matrix4::getElement(const int& row, const int& col)
{
	return mElements[row*4 + col];
}
////////////////////////////////////////////////////////////////////

////Get rows and columns of a matrix.
Vector4 Matrix4::getRow(Vector4::AXIS axisRow) const
{
	return Vector4(mElements[axisRow*4], mElements[axisRow*4+1], mElements[axisRow*4+2], mElements[axisRow*4+3]);
}

Vector4 Matrix4::getColumn(Vector4::AXIS axisRow) const
{
	return Vector4(mElements[axisRow], mElements[axisRow+4], mElements[axisRow+8], mElements[axisRow+12]);
}

const float& Matrix4::operator[](unsigned int index) const
{
	return mElements[index];
}
	
float& Matrix4::operator[](unsigned int index) 
{
	return mElements[index];
}

const float& Matrix4::operator()(unsigned int r, unsigned int c)
{
	return mElements[r*4 + c];
}

bool Matrix4::getInverse(Matrix4& invOut)
{
	float inv[16], det;
	int i;

	inv[0] = mElements[5]  * mElements[10] * mElements[15] - 
				mElements[5]  * mElements[11] * mElements[14] - 
				mElements[9]  * mElements[6]  * mElements[15] + 
				mElements[9]  * mElements[7]  * mElements[14] +
				mElements[13] * mElements[6]  * mElements[11] - 
				mElements[13] * mElements[7]  * mElements[10];

	inv[4] = -mElements[4]  * mElements[10] * mElements[15] + 
				mElements[4]  * mElements[11] * mElements[14] + 
				mElements[8]  * mElements[6]  * mElements[15] - 
				mElements[8]  * mElements[7]  * mElements[14] - 
				mElements[12] * mElements[6]  * mElements[11] + 
				mElements[12] * mElements[7]  * mElements[10];

	inv[8] = mElements[4]  * mElements[9] * mElements[15] - 
				mElements[4]  * mElements[11] * mElements[13] - 
				mElements[8]  * mElements[5] * mElements[15] + 
				mElements[8]  * mElements[7] * mElements[13] + 
				mElements[12] * mElements[5] * mElements[11] - 
				mElements[12] * mElements[7] * mElements[9];

	inv[12] = -mElements[4]  * mElements[9] * mElements[14] + 
				mElements[4]  * mElements[10] * mElements[13] +
				mElements[8]  * mElements[5] * mElements[14] - 
				mElements[8]  * mElements[6] * mElements[13] - 
				mElements[12] * mElements[5] * mElements[10] + 
				mElements[12] * mElements[6] * mElements[9];

	inv[1] = -mElements[1]  * mElements[10] * mElements[15] + 
				mElements[1]  * mElements[11] * mElements[14] + 
				mElements[9]  * mElements[2] * mElements[15] - 
				mElements[9]  * mElements[3] * mElements[14] - 
				mElements[13] * mElements[2] * mElements[11] + 
				mElements[13] * mElements[3] * mElements[10];

	inv[5] = mElements[0]  * mElements[10] * mElements[15] - 
				mElements[0]  * mElements[11] * mElements[14] - 
				mElements[8]  * mElements[2] * mElements[15] + 
				mElements[8]  * mElements[3] * mElements[14] + 
				mElements[12] * mElements[2] * mElements[11] - 
				mElements[12] * mElements[3] * mElements[10];

	inv[9] = -mElements[0]  * mElements[9] * mElements[15] + 
				mElements[0]  * mElements[11] * mElements[13] + 
				mElements[8]  * mElements[1] * mElements[15] - 
				mElements[8]  * mElements[3] * mElements[13] - 
				mElements[12] * mElements[1] * mElements[11] + 
				mElements[12] * mElements[3] * mElements[9];

	inv[13] = mElements[0]  * mElements[9] * mElements[14] - 
				mElements[0]  * mElements[10] * mElements[13] - 
				mElements[8]  * mElements[1] * mElements[14] + 
				mElements[8]  * mElements[2] * mElements[13] + 
				mElements[12] * mElements[1] * mElements[10] - 
				mElements[12] * mElements[2] * mElements[9];

	inv[2] = mElements[1]  * mElements[6] * mElements[15] - 
				mElements[1]  * mElements[7] * mElements[14] - 
				mElements[5]  * mElements[2] * mElements[15] + 
				mElements[5]  * mElements[3] * mElements[14] + 
				mElements[13] * mElements[2] * mElements[7] - 
				mElements[13] * mElements[3] * mElements[6];

	inv[6] = -mElements[0]  * mElements[6] * mElements[15] + 
				mElements[0]  * mElements[7] * mElements[14] + 
				mElements[4]  * mElements[2] * mElements[15] - 
				mElements[4]  * mElements[3] * mElements[14] - 
				mElements[12] * mElements[2] * mElements[7] + 
				mElements[12] * mElements[3] * mElements[6];

	inv[10] = mElements[0]  * mElements[5] * mElements[15] - 
				mElements[0]  * mElements[7] * mElements[13] - 
				mElements[4]  * mElements[1] * mElements[15] + 
				mElements[4]  * mElements[3] * mElements[13] + 
				mElements[12] * mElements[1] * mElements[7] - 
				mElements[12] * mElements[3] * mElements[5];

	inv[14] = -mElements[0]  * mElements[5] * mElements[14] + 
				mElements[0]  * mElements[6] * mElements[13] + 
				mElements[4]  * mElements[1] * mElements[14] - 
				mElements[4]  * mElements[2] * mElements[13] - 
				mElements[12] * mElements[1] * mElements[6] + 
				mElements[12] * mElements[2] * mElements[5];

	inv[3] = -mElements[1] * mElements[6] * mElements[11] + 
				mElements[1] * mElements[7] * mElements[10] + 
				mElements[5] * mElements[2] * mElements[11] - 
				mElements[5] * mElements[3] * mElements[10] - 
				mElements[9] * mElements[2] * mElements[7] + 
				mElements[9] * mElements[3] * mElements[6];

	inv[7] = mElements[0] * mElements[6] * mElements[11] - 
				mElements[0] * mElements[7] * mElements[10] - 
				mElements[4] * mElements[2] * mElements[11] + 
				mElements[4] * mElements[3] * mElements[10] + 
				mElements[8] * mElements[2] * mElements[7] - 
				mElements[8] * mElements[3] * mElements[6];

	inv[11] = -mElements[0] * mElements[5] * mElements[11] + 
				mElements[0] * mElements[7] * mElements[9] + 
				mElements[4] * mElements[1] * mElements[11] - 
				mElements[4] * mElements[3] * mElements[9] - 
				mElements[8] * mElements[1] * mElements[7] + 
				mElements[8] * mElements[3] * mElements[5];

	inv[15] = mElements[0] * mElements[5] * mElements[10] - 
				mElements[0] * mElements[6] * mElements[9] - 
				mElements[4] * mElements[1] * mElements[10] + 
				mElements[4] * mElements[2] * mElements[9] + 
				mElements[8] * mElements[1] * mElements[6] - 
				mElements[8] * mElements[2] * mElements[5];

	det = mElements[0] * inv[0] + mElements[1] * inv[4] + mElements[2] * inv[8] + mElements[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0f / det;

	for (i = 0; i < 16; i++)
		invOut[i] = inv[i] * det;

	return true;
}	



Vector4 operator*(const Matrix4& matrix, const Vector4& vec)
{
	//@TODO: Actually hardcode the calculation.  It will be faster.
	return Vector4(Vector4::dot(matrix.getRow(Vector4::X), vec), 
					Vector4::dot(matrix.getRow(Vector4::Y), vec), 
					Vector4::dot(matrix.getRow(Vector4::Z), vec), 
					Vector4::dot(matrix.getRow(Vector4::W), vec) );
}