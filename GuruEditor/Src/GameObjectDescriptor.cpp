/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       GameObjectDescriptor.cpp
Brief:      
/************************************************************************/

#include "GameObjectDescriptor.h"




GameObjectDescriptor::GameObjectDescriptor()
{
}


GameObjectDescriptor::~GameObjectDescriptor()
{
}


void GameObjectDescriptor::AddComponentDescriptor(ComponentDescriptor* descriptor)
{
	mComponentDescriptors.push_back(descriptor);
}
