
//////////////////////////////////////////////////////////////////////////
//
//////////////////////////////////////////////////////////////////////////

#include "Game_Picker.h"
#include "Game_Engine.h"
#include "Primitives.h"
#include "WorldScene.h"
#include <iostream>
#include <cmath>

GamePicker::GamePicker() : GPicker()
{

}

GamePicker::~GamePicker()
{
}

//Goes through the game engine scene graph directly.
GameObject* GamePicker::PickSceneGraphSingleObject(const int& px, const int& py)
{
	//Have mouse coords.  First, define the view plane.
	Camera* camera = Game_Engine::GetSingletonPtr()->Module()->GetCamera();
	CVector3 cameraLook = camera->Look();
	CVector3 cameraPos = camera->Position();
	CVector3 cameraSide = camera->Side();
	CVector3 cameraUp = camera->Up();

	//Up and side form the view plane.  THe x and y coords can be transformed into a 
	// parameterized world coord with the plane equation.
	float fovyRad = camera->FieldOfView() * (M_PI / 180.0f);
	float width = camera->Width();  //std::tan(fovyRad) * camera->NearDist();
	float height = camera->Height();  //width * camera->AspectRatio();

	//Now generate the point on the view plane that is the world coords of the mouse in screen space.

	//transform mouse coords so origin is center, not top-left.
	float x = (float)px - ((float)width / 2.0f);
	float y = (float)py - ((float)height / 2.0f);

	//scale so width/height are each 1.
	x /= width;
	y /= height;

	CVector3 worldCoords = cameraPos + cameraLook * camera->NearDist() + (cameraSide * x + cameraUp * y);
	CVector3 worldRayDir = worldCoords - cameraPos;
	worldRayDir.normalize();

	mRay = Ray(worldCoords, worldRayDir);

	//Go through all the objects
	Game_Engine* engine = (Game_Engine*)Game_Engine::GetSingletonPtr();
	auto beginIter = engine->mWorldScene->mObjects->begin();
	auto endIter = engine->mWorldScene->mObjects->end();

	CVector3 inPoint;
	
	while(beginIter != endIter)
	{
		if( intersectRaySphere(mRay, beginIter->second->mPickSphere, inPoint) )
		{
			//Set the picked object.
			mCurPickedObject = beginIter->second;
			mLastPickWorldCoords = inPoint;
			mClicked = inPoint;
			return (GameObject*)mCurPickedObject;
		}
		++beginIter;
	}

	return NULL;
}


//Picks scene graph for all objects hit, and returns the closest one.  rAllObjects contains all objects hit, including closest.
GameObject* GamePicker::PickSceneGraphMultipleObjects(const int& px, const int& py, std::vector<GameObject*>& rAllObjects)
{
	return NULL;
}


Entity*	GamePicker::PickSingleObject( const int& pX, const int& pY, const std::vector<Entity*>	SceneObjs )
{
	GPicker::PickSingleObject(pX, pY, SceneObjs );
	return mCurPickedObject;
}

Entity*	GamePicker::ConnectSceneSingleObject( const int& /*pX*/, const int& /*pY*/, const std::vector<Entity*>	SceneObjs )
{
	return mCurPickedObject;
}


//////////////////////////////////////////////////////////////////////////
CVector3 GamePicker::PickAndCreateWaypoint( const int& x, const int& y )
{
	Ray ray = ComputePickerData(x, y);

	//////////////////////////////////////////////////////////////////////////	
	//@TODO: Create the waypoint and connect it to all other it's not occluded from.
	//////////////////////////////////////////////////////////////////////////

	return mClicked;
}

CVector3 GamePicker::PickAndCreateXObject( const int& x, const int& y )
{
	Ray ray = ComputePickerData(x, y);

	//////////////////////////////////////////////////////////////////////////	
	//@TODO: Create the model and add it to the render scene.
	//////////////////////////////////////////////////////////////////////////

	//At this point mClicked will now be set to some point in space where the object center was created.
	return mClicked;
}
