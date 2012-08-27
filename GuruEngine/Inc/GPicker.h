
//////////////////////////////////////////////////////////////////////////
// Daniel Strycharske
// 08.14.12
// GPicker - Declaration of a class to help assist with grabbing and moving objects.
//////////////////////////////////////////////////////////////////////////


#ifndef _GURU_GPICKER_H_
#define _GURU_GPICKER_H_

#include <vector>
#include "Entity.h"
#include "Primitives.h"
#include "gl_inc.h"


class GPicker
{
//////////////////////////////////////////////////////////////////////////
// PUBLIC CONSTRUCTION INTERFACE
//////////////////////////////////////////////////////////////////////////
public:
	GPicker();
	virtual ~GPicker();

//////////////////////////////////////////////////////////////////////////
// PRIVATE INITTIALIZATION INTERFACE
//////////////////////////////////////////////////////////////////////////
private:
	//Used to initialize the picker to default values.  Not public because this could reset a valid
	// picker if called after creation/updating.
	void Init();

//////////////////////////////////////////////////////////////////////////
// PURE VIRTUAL PUBLIC INTERFACE
//////////////////////////////////////////////////////////////////////////
public:
	//Function taking a screen position, forms a ray, shoots it into view or world coords, and returns
	//  the first object it hits.  Returns NULL if no object hit.
	virtual Entity*	PickSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs ) = 0;
	virtual Entity*	ConnectSceneSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs ) = 0;
			
	virtual CVector3 PickAndCreateWaypoint( const int& x, const int& y ) = 0;
	virtual CVector3 PickAndCreateXObject( const int& x, const int& y ) = 0;
	
	virtual Entity* GetPickedObject()	{ return mCurPickedObject; }
	virtual int	GetPickingMode()	{ return mPickingMode; }

//////////////////////////////////////////////////////////////////////////
// PUBLIC STATIC INTERFACE
//////////////////////////////////////////////////////////////////////////
public:
	static CVector3	UnprojectPoint( const CVector3&	point );

//////////////////////////////////////////////////////////////////////////
// PROTECTED HELPER METHODS FOR DERIVED CLASSES
//////////////////////////////////////////////////////////////////////////
protected:
	//Function that will set return parameters for the start point and direction for the picking, but doesn't
	// actually cast the ray out.
	Ray ComputePickerData( const int& pX, const int& pY ); 
	virtual void SetPickingMode(int pPickingMode)	{ mPickingMode = pPickingMode; }
	virtual void SetPickedObject( Entity* ptr )	{ mCurPickedObject = ptr; }

//////////////////////////////////////////////////////////////////////////
// PROTECTED CLASSES/STRUCTS, ACCESSED BY DERIVED CLASSES
//////////////////////////////////////////////////////////////////////////
protected:

//////////////////////////////////////////////////////////////////////////
// PROTECTED MEMBER DATA, ACCESSED BY DERIVED CLASSES
//////////////////////////////////////////////////////////////////////////
protected:
	//Needs to have the current picked point values, in screen, view, and world coordinates.
	int mScreenX, mScreenY;

	//The current picker ray in view and world space.
	Ray				mRay;
	CVector3		mRayStart;
	CVector3		mRayDirection;


	//Point in space representing where either an object or the ground plane was clicked.
	CVector3		mClicked;

	//Type of mode it's in, if it's in select only or select and transform.
	int				mPickingMode;

	Entity*			mCurPickedObject;
	std::vector<Entity*> mAllPickedObjects;
};




#endif
