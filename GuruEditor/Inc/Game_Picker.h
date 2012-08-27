
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////


#ifndef _GURU_GAME_PICKER_H_
#define _GURU_GAME_PICKER_H_


#include "GPicker.h"

class GameObject;

class GamePicker : public GPicker
{
public:
	GamePicker();
	virtual ~GamePicker();

	//Goes through the game engine scene graph directly.
	GameObject* PickSceneGraphSingleObject(const int& px, const int& py);
	//Picks scene graph for all objects hit, and returns the closest one.  rAllObjects contains all objects hit, including closest.
	GameObject* PickSceneGraphMultipleObjects(const int& px, const int& py, std::vector<GameObject*>& rAllObjects);

	//The game app level will need to implement the 
	Entity*	PickSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs );
	Entity*	ConnectSceneSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs );

	//////////////////////////////////////////////////////////////////////////
	CVector3 PickAndCreateWaypoint( const int& x, const int& y );
	CVector3 PickAndCreateXObject( const int& x, const int& y );
		
	CVector3 MouseWorldCoords();
	CVector3 RayDirection();
	Ray	getPickRay();
	Entity* getLastPickedObject();
	std::vector<Entity*> getLastPickedGroup();

private:
	//////////////////////////////////////////////////////////////////////////
	// Standard, 3D world view ray-object picking.
	//////////////////////////////////////////////////////////////////////////
	CVector3 mLastPickDirection;
	CVector3 mLastPickWorldCoords;
	Entity* mLastPickedObject;
	std::vector<Entity*> mLastPickedGroup;
};

#endif
