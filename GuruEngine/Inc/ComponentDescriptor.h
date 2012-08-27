/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       ComponentDescriptor.h
Brief:      
/************************************************************************/

#ifndef ComponentDescriptor_H
#define ComponentDescriptor_H


/************************************************************************/
/* Includes
/************************************************************************/
#include <string>
#include <cstdlib>
#include <map>
#include "Entity.h"


/************************************************************************/
/* Forward Declarations
/************************************************************************/
class ComponentDisciple;
enum ComponentFamily;


class ComponentDescriptor
{
public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	ComponentDescriptor();
	virtual ~ComponentDescriptor();	
	virtual ComponentDisciple* Create(Entity* obj) = 0;

	/************************************************************************
	*
	************************************************************************/	

	/************************************************************************
	* Protected - Member variables
	************************************************************************/
	std::string mType;
	std::string mName;
	ComponentFamily mFamily;
	Entity* mParent;
};


#endif
