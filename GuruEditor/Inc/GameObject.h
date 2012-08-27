/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef GURU_GAMEOBJECT_H
#define GURU_GAMEOBJECT_H



#include "PhysicsComponent.h"
#include "RenderComponent.h"
#include "Entity.h"


class GameObjectDescriptor;
class GameObjectFactory;


class GameObject : public Entity
{
	friend class GameObjectFactory;
	/********************************************************************************
	    Class: GameObject -> Entity
		Brief: 
	/********************************************************************************/
	
private:
	/************************************************************************/
	/* Maintaining script references
	/************************************************************************/
	int mObjectRef;

public:
	GameObject();
	GameObject(const GameObject& object);
	virtual ~GameObject();

	/************************************************************************/
	/* Virtual - Entity
	/************************************************************************/
	//virtual void Init(EntityDescriptor* const descriptor);
	virtual void SetUpComponentConnections();
	
	/************************************************************************/
	/* Virtual - DObject
	/************************************************************************/
	DObject* Deserialize() { return NULL; }
	virtual bool Serialize( std::fstream* ) { return true; }
	virtual bool Serialize( std::iostream* ) { return true; }
	virtual bool Serialize( FILE* ) { return true; }
	virtual bool Serialize( char* , size_t ) { return true; }

	/************************************************************************/
	/* Retrieved the components.  By default grabs typical naming convention
	/************************************************************************/
	ComponentDisciple*	GetGraphicsComp(std::string name="Main", std::string type = "Model");
	ComponentDisciple*	GetPhysicsComp(std::string name="Main", std::string type = "RigidBody");
	//@TODO:  Perhaps have default for each of the 

	/************************************************************************/
	/* For the script reference binding.
	/************************************************************************/
	int GetScriptObjRef() { return mObjectRef; }
	void SetScriptObjRef(int objref ) { mObjectRef = objref; }
	
public:
	/************************************************************************/
	/* Static - Component creation functions
	/************************************************************************/
	//static std::shared_ptr<ComponentDisciple> CreatePhysicsComponent(std::string name="Main", std::string type = "RigidBody");
	//static std::shared_ptr<ComponentDisciple> CreateRenderComponent(std::string name="Main", std::string type = "Model");

#ifdef USE_LUA
	LUA_DECLARE_CLASS( GameObject )
		LUA_DECLARE_METHOD( LCreatePhysicsComponent )
		LUA_DECLARE_METHOD( LCreateRenderComponent )
		LUA_DECLARE_METHOD( LGetPhysicsComp )
		LUA_DECLARE_METHOD( LGetRenderComp )
		LUA_DECLARE_METHOD( LSetScriptObjRef )
#endif //if using Lua
};

void findAndAddComponentsOfFamily(GameObject* obj, ComponentDisciple* component, ComponentFamily compFamily, ComponentConnectionDir accessType);

typedef GameObject* GameObjectPtr;


#endif
