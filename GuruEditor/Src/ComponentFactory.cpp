/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       ComponentFactory.cpp
Brief:      ComponentFactory class implementation.
/************************************************************************/


#include "ComponentFactory.h"
#include "ComponentDescriptor.h"


ComponentFactory::ComponentFactory()
{
}


ComponentFactory::~ComponentFactory()
{
}


/********************************************************************************
    Function: GuruComponentFactory::Create
	Params:   shared_ptr<ComponentDescriptor>
	Return:   shared_ptr<ComponentDisciple>
	Brief:    The workhorse function for the factory.  It takes the descriptors member data
				and generates a component from it, depending on the desired component type.
/********************************************************************************/
ComponentDisciple* ComponentFactory::Create(Entity* obj, ComponentDescriptor* description)
{	
	return description->Create(obj);
}
