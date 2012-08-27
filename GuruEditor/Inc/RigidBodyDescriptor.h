/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       RigidBodyDescriptor.h
Brief:      
/************************************************************************/

#ifndef RigidBodyDescriptor_H
#define RigidBodyDescriptor_H


/************************************************************************/
/* Includes
/************************************************************************/
#include "ComponentDescriptor.h"
#include "Utility.h"
#include "DQuaternion.h"


/************************************************************************/
/* Forward Declarations
/************************************************************************/
class Shape;
class RigidBodyComponent;
class ComponentDisciple;


class RigidBodyDescriptor : public ComponentDescriptor
{
	struct SpatialDesc
	{
		CVector3 mPosition;
		DQuaternion mOrientation;
	};

	struct PhysicalDesc
	{
		std::vector< Shape* > mShapes;
		float mFriction;
		float mRestitution;
	};

	struct MassDesc
	{
		float mMass;
		float mIMass;
		float mDensity;
	};

private:
	/************************************************************************/
	/* Private - Member Variables
	/************************************************************************/
	SpatialDesc mSpatialData;
	PhysicalDesc mPhysicalData;
	MassDesc mMassData;

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
	RigidBodyDescriptor();
	~RigidBodyDescriptor();

	/************************************************************************/
	/* Public - Member methods
	/************************************************************************/
		
	/************************************************************************/
	/* Public - Member Variables
	/************************************************************************/

	/************************************************************************/
	/* Public - Static Member methods
	/************************************************************************/
	virtual ComponentDisciple* Create(Entity* obj);
};


#endif
