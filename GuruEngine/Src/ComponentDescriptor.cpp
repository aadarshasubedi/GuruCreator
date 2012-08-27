/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       ComponentDescriptor.cpp
Brief:      
/************************************************************************/


#include "ComponentDescriptor.h"
#include "ComponentDisciple.h"


ComponentDescriptor::ComponentDescriptor() : mType(""), mName(""), mFamily(Undefined), mParent(NULL)
{
}


ComponentDescriptor::~ComponentDescriptor()
{
}
