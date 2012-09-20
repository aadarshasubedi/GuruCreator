/************************************************************************
Author:		Dan Strycharske
Date:       09.05.09
File:       GameObjectFactory.cpp
Brief:      Factory for creating, maintaining, and deleting GameObjects.
/************************************************************************/


#include "GameObjectFactory.h"
#include "GameObject.h"
#include "GameObjectDescriptor.h"
#include "ComponentFactory.h"
#include "ModelComponent.h"
#include <algorithm>


/********************************************************************************
    Function: GameObjectFactory::GameObjectFactory
	Params:    
	Return:   
	Brief:    
/********************************************************************************/
GameObjectFactory::GameObjectFactory() : mObjects()
{
	mComponentFactory = new ComponentFactory;
}


/********************************************************************************
    Function: GameObjectFactory::~GameObjectFactory
	Params:    
	Return:   
	Brief:    Destructor.  Actually don't need to do anything because once 
				this goes out of scope with the containing World, it will be cleaned up.
/********************************************************************************/
GameObjectFactory::~GameObjectFactory()
{
}


//This will store all of the game objects in the game in a linked list.  Deleting is trivial as the list is re-linked and the object freed of it's memory.
//std::shared_ptr<GameObject> GameObjectFactory::CreateGameObject(const GameObjectDescriptor& description)
GameObject* GameObjectFactory::CreateGameObject(GameObjectDescriptor description)
{
	auto objPtr = new GameObject;
	objPtr->mPosition = description.mPosition;
	objPtr->mPickSphere.setCenter(description.mPosition);
	objPtr->mOrientation = description.mOrientation;
		
	//Go through all of the descriptions component descriptions and call the component factory.
	std::for_each(begin(description.mComponentDescriptors), end(description.mComponentDescriptors), [&](ComponentDescriptor* desc)
	{
		//Here we push all the real hard work off onto the factory and just get back a nice ref counted pointer to our component.

		//Add this component to the game object.
		auto comp = mComponentFactory->Create(objPtr, desc);
		objPtr->AddComponent( comp );
	});

	//This sets up the initial implicit bi-directional graph of which component can talk to who directly w/o events or messages of any kind; direct pointer access.
	//objPtr->SetUpComponentConnections();
		
	mObjects.insert(std::make_pair(objPtr->GetGUID(), objPtr));
	return objPtr;
}


bool GameObjectFactory::DeleteGameObject(GameObjectPtr obj)
{
	auto deathRow = mObjects.find(obj->GetGUID());
	if( deathRow != mObjects.end() )
	{	
		mObjects.erase(deathRow);
		return true;
	}

	//Something bad, no object to delete.
	return false;
	//From here the shared_ptr will call the destructor if this was the last ref.
	//Since this is where they are persistently stored, the only other refs will should be temporary.
}


GameObjectPtr GameObjectFactory::GetLastObjectCreated() const
{
	//If size is zero, return NULL.  Otherwise return the last element.
	unsigned int size = mObjects.size();
	auto ender = mObjects.end();
	--ender;

	return (size) ? ender->second : NULL;
}
