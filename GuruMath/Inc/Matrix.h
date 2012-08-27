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

//The matrix in implementation is fairly lightweight, unless using very large matrices.  It is simply width and height, defining
// the number of rows and columns to the matrix.  Matrices will be by default column major, but with conversion functions to row-major.
template <typename T=float, unsigned int C=4, unsigned int R=4>
class Matrix
{
public:
	Matrix(unsigned int cols = C, unsigned int rows = R); //By default, a 4x4 will be created.
	Matrix(const Matrix& B);
	virtual ~Matrix();

protected:
	//The array of values.
	T* mElements;

	//The column and row values.
	const unsigned int mColumns;
	const unsigned int mRows;

public:

	///ACCESSORS
	const T& operator[](unsigned int index) const;
	const T& operator()(unsigned int index) const;
	const T& operator()(unsigned int col, unsigned int row) const;
	const unsigned int numRows() const { return mRows; }
	const unsigned int numColumns() const { return mColumns; }

	///MUTATORS
	T& operator[](unsigned int index);
	T& operator()(unsigned int index);
	T& operator()(unsigned int col, unsigned int row);
	void updateValue(unsigned int index, const T& value);
	void updateValue(unsigned row, unsigned col, const T& value);
	void updateMatrix(const T* value);  //assumes the size of array value is the same as the columns*rows.

	///OPERATIONS
	//For generic matrix operation A*B=C where A = m x n and B = j x k, n == j && m == k, otherwise the op can't be performed.
	Matrix operator*(const Matrix& B);  // *this = A
	//Scalar multiplication is even more trivial.
	Matrix operator*(const T& scalar);

	//Addition is trivial.  Implement subtraction by leveraging this function.
	Matrix operator+(const Matrix& B);

	//Subtraction, see addition.
	Matrix operator-(const Matrix& B);

	//Setting
	void operator=(const Matrix& B);

	Matrix operator-();

	//Division does not exist.
	void copy(const Matrix& B);

	//@TODO: The last 6 need finishing, excpet 'det' which just calls determinant.
	//Finding the determinent is a crucially helpful thing.
	T determinant();
	T det(); //This just calls determinant().  Helpful as a shorthand.

	//Inverse, transpose methods.
	void invert();  //This will physically change the matrix to be the inverse.
	Matrix<T,C,R> getInverse(); //This will find the inverse and return it without altering the current Matrix.
	
	void transpose();
	Matrix<T,C,R> getTranspose();

public:	
	//Define several helper functions for printing and testing matrices.
	static void testMatrix();
	static void printMatrix( const Matrix& mat );

	//....other needed functions for a matrix.
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R>::Matrix(unsigned int cols, unsigned int rows) : mColumns(cols), mRows(rows)
{
	mElements = (mColumns*mRows > 0) ? new T[mColumns * mRows] : NULL;
	for( unsigned int r_iter = 0; r_iter < mRows; ++r_iter )
	{
		for( unsigned int c_iter = 0; c_iter < mColumns; ++c_iter )
		{
			mElements[r_iter * mColumns + c_iter] = 0;
		}
	}
}


template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R>::Matrix(const Matrix& B) : mColumns(B.numColumns()), mRows(B.numRows()), mElements(NULL)
{
	copy(B);
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::copy(const Matrix<T,C,R>& B)
{	
	if( B.numRows() == 0 || B.numColumns() == 0 )
	{
		//Actually, just do nothing to the matrix.
	}
	else
	{
		if( mElements ) delete [] mElements;
		mElements = new T[mColumns * mRows];
		for( unsigned row = 0; row < mRows; ++row )
		{
			for( unsigned int col = 0; col < mColumns; ++col )
			{
				mElements[row * mColumns + col] = B[row * mColumns + col];
			}
		}
	}
}


template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R>::~Matrix()
{
	delete [] mElements;
}



///ACCESSORS
template <typename T,unsigned int C, unsigned int R>
const T& Matrix<T,C,R>::operator[](unsigned int index) const
{
	if( (index > (mColumns*mRows)-1) || index < 0 )
	{
		//Bad, throw an exception.
	}
	return mElements[index];
}


template <typename T,unsigned int C, unsigned int R>
const T& Matrix<T,C,R>::operator()(unsigned int index) const
{
	if( (index > (mColumns*mRows)-1) || index < 0 )
	{
		//Bad, throw an exception.
	}
	return mElements[index];
}


template <typename T,unsigned int C, unsigned int R>
const T& Matrix<T,C,R>::operator()(unsigned int col, unsigned int row) const
{
	if( col < 0 || col > mColumns || row < 0 || row > mRows ) { return 0; }//THis is bad. 

	unsigned int index = row * mColumns + col;
	return mElements[index];
}


///MUTATORS
template <typename T,unsigned int C, unsigned int R>
T& Matrix<T,C,R>::operator[](unsigned int index)
{
	if( (index > (mColumns*mRows)-1) || index < 0 )
	{
		//Bad, throw an exception.
	}
	return mElements[index];
}


template <typename T,unsigned int C, unsigned int R>
T& Matrix<T,C,R>::operator()(unsigned int index)
{
	if( (index > (mColumns*mRows)-1) || index < 0 )
	{
		//Bad, throw an exception.
	}
	return mElements[index];
}

template <typename T,unsigned int C, unsigned int R>
T& Matrix<T,C,R>::operator()(unsigned int col, unsigned int row)
{
	if( col < 0 || col > mColumns || row < 0 || row > mRows ) { return mElements[0]; }//THis is bad. 

	unsigned int index = row * mColumns + col;
	return mElements[index];
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::updateValue(unsigned int index, const T& value)
{
	if( (index > (mColumns*mRows)-1) || index < 0 )
	{
		//Bad, throw an exception.
	}
	mElements[index] = value;
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::updateValue(unsigned int row, unsigned int col, const T& value)
{
	if( row > mRows*mColumns || col > mRows*mColumns || row < 0 || col < 0 )
	{
		//Bad, throw an exception.
	}
	mElements[row*mColumns + col] = value;
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::updateMatrix(const T* value)  //assumes the size of array value is the same as the columns*rows.
{
	for( unsigned row = 0; row < mRows; ++row )
	{
		for( unsigned col = 0; col < mColumns; ++col )
		{
			unsigned int index = row*mColumns + col;
			mElements[index] = value[index];
		}
	}
}


//For generic matrix operation A*B=C where A = m x n and B = j x k, n == j && m == k, otherwise the op can't be performed.
template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::operator*(const Matrix<T,C,R>& B)
{
	//dot product, dot product, dot product

	//Make sure that the cols X rows of A are rows X cols for B then we can do operation.  If not, then exception.
	//This checks that the matrices are symmetric...
	if( (numRows() != B.numColumns() || numColumns() != B.numRows()) && 
		//This is the special vector multiplication case, will have one column and matrix A must be square
		(B.numColumns() != 1 && B.numRows() != numColumns() && numColumns() != numRows()) )
	{
		//@TODO: Badness, exception.
		return NullMat;
	}

	//The size of the final matrix is A.rows X B.columns	
	T* values = new T[numRows() * B.numColumns()];

	//for every column in B, multiply each row in A.
	for( unsigned int c_iter = 0; c_iter < B.numColumns(); ++c_iter )
	{
		T* b_vector = new T[B.numRows()];

		//For each column, there are B.numRows() values in the vector.
		for( unsigned int r_iter = 0; r_iter < B.numRows(); ++r_iter )
		{
			//Grab the next row value.  The reason to do it this way is because the matrix size can be variable.
			b_vector[r_iter] = B[r_iter * B.numColumns() + c_iter];
		}

		//Have vector, multiply against A's rows.
		for( unsigned int r_iter = 0; r_iter < numRows(); ++r_iter )
		{
			T newValue = 0;

			//go through each value in the bvector and multiply it to the appropriate element in A, summing them all.
			for( unsigned int b_iter = 0; b_iter < B.numRows(); ++b_iter )
			{
				newValue += mElements[r_iter * mColumns + b_iter] * b_vector[b_iter];
			}

			//Generates the entire new column for the final matrix.	
			values[B.numColumns() * r_iter + c_iter] = newValue;
		}
		delete b_vector;
	}

	Matrix<T> temp(B.numColumns(), numRows());
	temp.updateMatrix( values );

	delete values;

	return temp;
}


//Scalar multiplication is even more trivial.
template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::operator*(const T& scalar)
{
	Matrix<T> temp = *this;

	for( unsigned col = 0; col < mColumns; ++col )
	{
		for( unsigned row = 0; row < mRows; ++row )
		{
			temp[row * mColumns + col] *= scalar;
		}
	}

	return temp;
}


//Addition is trivial.  Implement subtraction by leveraging this function.
template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::operator+(const Matrix<T,C,R>& B)
{
	if( mColumns != B.numColumns() || mRows != B.numRows() )
	{
		return NullMat;
	}

	Matrix<T,C,R> temp(mColumns, mRows);

	for( unsigned row = 0; row < mRows; ++row )
	{
		for( unsigned col = 0; col < mColumns; ++col )
		{
			unsigned int i = row * mColumns + col;
			temp[i] = mElements[i] + B[i];
		}
	}
	return temp;
}


//Subtraction, see addition.
template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::operator-(const Matrix<T,C,R>& B)
{
	if( mColumns != B.numColumns() || mRows != B.numRows() )
	{
		return NullMat;
	}	

	Matrix<T,C,R> temp(mColumns, mRows);	
	temp = -(*this) + B;
	return temp;
}


template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::operator-()
{
	for( unsigned row = 0; row < mRows; ++row )
	{
		for( unsigned col = 0; col < mColumns; ++col )
		{
			mElements[row * mColumns + col] = -mElements[row * mColumns + col];
		}
	}
	return *this;
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::operator=(const Matrix<T,C,R>& B)
{
	//@TODO: Finish this when needed, if needed.
	if( B.numColumns() != numColumns() && B.numRows() != numRows() )
	{
		//If the sizes don't match, don't alter either matrix.
	}
	else
	{
		//Make sure the column and rows aren't zero.  
		if( mColumns == 0 || mRows == 0 )
		{
			//Fall through and do nothing, this is a null matrix or degenerate matrix being set to another null or degenerate, do nothing.
		}
		else
		{
			copy(B);
		}
	}
}


//@TODO:  Finish this one up, this will be most difficult.
//Finding the determinent is a crucially helpful thing.
template <typename T,unsigned int C, unsigned int R>
T Matrix<T,C,R>::determinant()
{
	return T();
}


template <typename T,unsigned int C, unsigned int R>
T Matrix<T,C,R>::det()
{
	return determinant();
}


//@TODO:  Don't think about this, find algorithm and implement
//Inverse, transpose methods.
template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::invert()
{
	//WARNING!  This physically inverts the current matrix.  Use only when you want to actually change a matrix to its inverse,
	//  use 'inverse' if you want to simply get the inverse w/o affecting the original matrix.
}


//@TODO:  Don't think about this, find algorithm and implement
template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::getInverse() //This will find the inverse and return it without altering the current Matrix.
{
	return Matrix<T,C,R>();
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::transpose()
{
}


template <typename T,unsigned int C, unsigned int R>
Matrix<T,C,R> Matrix<T,C,R>::getTranspose()
{
	return Matrix<T,C,R>;
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::printMatrix(const Matrix<T,C,R>& mat)
{
	printf("Matrix: (%d x %d)\n", mat.numColumns(), mat.numRows());
	for( unsigned int i = 0; i < mat.numRows(); ++i )
	{
		for( unsigned int j = 0; j < mat.numColumns(); ++j )
		{
			unsigned int index = i * mat.numColumns() + j;
			printf("Index: %d - %f\n", index, mat(index) );
		}
	}
	printf("\n");
}


template <typename T,unsigned int C, unsigned int R>
void Matrix<T,C,R>::testMatrix()
{
	Matrix4 A,B;

	float mat[16] = { 1, 0, 2, 0,
					  0, 1, 5, 7, 
					  3, 1, 4, 0, 
					  3, 4, 0, 1 };

	
	float ident[16] = { 1, 0, 0, 0,
					  0, 1, 0, 0, 
					  0, 0, 1, 0, 
					  0, 0, 0, 1 };

	A.updateMatrix(mat);		
	printMatrix(A);

	A.updateMatrix(ident);
	printMatrix(A*4);

	A = A + A;
	printMatrix(A);
	
	float mat2[16] = { 1, 5, 0, 2,
					  0, 1, 2, 0, 
					  4, 0, 1, 9, 
					  0, 7, 0, 1 };

	B.updateMatrix(mat2);

	Matrix4 C = A - B;
	printMatrix(C);


	//Different size matrices.
	Matrix<float> twoByThree(1,3);
	Matrix<float> threeByTwo(3,1);

	float mat3[3] = {1,0,1};

	twoByThree.updateMatrix(mat3);
	threeByTwo.updateMatrix(mat3);
	printMatrix(twoByThree);
	printMatrix(threeByTwo);

	Matrix<> final = twoByThree * threeByTwo;
	printMatrix(final);

	final = twoByThree * A;
	printMatrix(final);

	Matrix<> badAdd = twoByThree + threeByTwo;
	printMatrix(badAdd);
}



//A utility of functions for doing conversions from/to quaternions, certain identities, etc.
typedef Matrix<float,4,4> Matrix4;

//An object to define a NULL matrix object.
const Matrix<float,0,0> NullMat(0,0);


#endif // GURU_MATRIX_H
