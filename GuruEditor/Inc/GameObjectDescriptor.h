/************************************************************************
Author:		Dan Strycharske
Date:       02.05.12
File:       GameObjectDescriptor.h
Brief:      
/************************************************************************/


#ifndef GameObjectDescriptor_H
#define GameObjectDescriptor_H


/************************************************************************/
/* Includes
/************************************************************************/
#include "EntityDescriptor.h"
#include "ComponentDescriptor.h"
#include "DQuaternion.h"
#include <cstdlib>
#include <vector>


/************************************************************************/
/* Forward Declarations
/************************************************************************/


class GameObjectDescriptor : public EntityDescriptor
{
private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Private - Member methods
	/************************************************************************/

protected:
	/************************************************************************/
	/* Protected - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Protected - Member methods
	/************************************************************************/

public:
	/************************************************************************
	* Constructors/Destructor
	************************************************************************/
	GameObjectDescriptor();
	~GameObjectDescriptor();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
	void AddComponentDescriptor(ComponentDescriptor* descriptor);

	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/
	std::vector< ComponentDescriptor* > mComponentDescriptors;

	virtual void init() {}

	CVector3 mPosition;
	CVector3 mRenderPosition;
	DQuaternion mOrientation;
};


#endif
