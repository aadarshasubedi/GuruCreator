/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       RigidBodyDescriptor.cpp
Brief:      RigidBodyDescriptor class implementation.
/************************************************************************/


#include "RigidBodyDescriptor.h"
#include "RigidBodyComponent.h"


RigidBodyDescriptor::RigidBodyDescriptor() : ComponentDescriptor()
{
}


RigidBodyDescriptor::~RigidBodyDescriptor()
{
}


ComponentDisciple* RigidBodyDescriptor::Create(Entity* obj)
{	
	RigidBodyComponent* comp = new RigidBodyComponent;

	//Set the values from one to the other.
	comp->SetFamily(mFamily);
	comp->SetName(mName);
	comp->SetType("RigidBody");
	comp->SetParent(obj);

	return comp;
}