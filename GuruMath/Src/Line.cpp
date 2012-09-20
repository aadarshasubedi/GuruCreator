/*
Author:		Dan Strycharske
Date:       02.01.12
File:       Line.cpp
Brief:      File containing the implementation details for Line.
*/

#include "Primitives.h"


//dir = B - A, and A is origin.
Ray Line::makeRay()
{
	return Ray(mA, mB-mA);
}
