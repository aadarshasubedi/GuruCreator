
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

	//Need the height to convert the y-coordinate.
	float height = camera->Height();

	//OpenGL expects (0,0) at bottom of window, not top like SDL uses.  
	int convertedY = height - py;

	//Set up the near and far points that will form the ray.
	CVector3 near, far;
	//All needed for the unprojection... matrices, and variables to store the results.
	GLdouble viewMat[16], projMat[16], nx, ny, nz, fx, fy, fz;
	//Viewport will also be needed.
	GLint viewport[4];

	glGetDoublev(GL_MODELVIEW_MATRIX, viewMat);
	glGetDoublev(GL_PROJECTION_MATRIX, projMat);
	glGetIntegerv(GL_VIEWPORT, viewport);

	gluUnProject(px, convertedY, 0.0, viewMat, projMat, viewport, &nx, &ny, &nz);
	gluUnProject(px, convertedY, 1.0, viewMat, projMat, viewport, &fx, &fy, &fz);

	//Set near and far points and set the ray.
	near.set(nx, ny, nz);
	far.set(fx, fy, fz);
	Ray ray(near, far - near);
	mRay.setOrigin(near);
	mRay.setDirection(far - near);
	
	//Go through all the objects
	Game_Engine* engine = (Game_Engine*)Game_Engine::GetSingletonPtr();
	auto beginIter = engine->mWorldScene->mObjects->begin();
	auto endIter = engine->mWorldScene->mObjects->end();

	CVector3 inPoint;
	
	while(beginIter != endIter)
	{
		if( intersectRaySphere(ray, beginIter->second->mPickSphere, inPoint) )
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
