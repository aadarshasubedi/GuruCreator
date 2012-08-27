/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

//
#include "GameObject.h"
#include "GameObjectDescriptor.h"
#include "WorldScene.h"
#include "RigidBodyDescriptor.h"
#include "RigidBodyComponent.h"
#include "ModelDescriptor.h"
#include "RenderComponent.h"
#include "ModelComponent.h"
#include "ComponentFactory.h"
#ifdef USE_LUA
#include "LunarHelper.h"
#endif
#include <algorithm>


/********************************************************************************
    Function: findAndAddComponentsOfFamily
	Params:   GameObject* obj, std::shared_ptr<ComponentDisciple> component, 
				ComponentFamily compFamily, ComponentConnectionDir accessType
	Return:   void
	Brief:    Uses 'obj's ComponentFamilyMapping to find all of the 'compFamily' type components.  
			  For all that are found, they are added to 'component' with 'accessType' access.
/********************************************************************************/
void findAndAddComponentsOfFamily(GameObject* obj, ComponentDisciple* component, 
									ComponentFamily compFamily, ComponentConnectionDir accessType)
{
	////Attaches to input and physics.
	//auto range = obj->getSiblingMapForWriting().equal_range(compFamily);

	//std::for_each( range.first, range.second, [&](std::pair<ComponentFamily, std::shared_ptr<ComponentDisciple> > it)
	//{
	//	//Graphics adds the physics one way.
	//	component->AddSiblingComponent(it.second, accessType);
	//});
}


GameObject::GameObject() : Entity()
{
#ifdef USE_LUA
	mObjectRef = LUA_NOREF;
#else //if not using Lua
	mObjectRef = 0;
#endif
}


GameObject::GameObject(const GameObject& object) : Entity(object)
{
}


GameObject::~GameObject()
{ 
}


void GameObject::SetUpComponentConnections()
{
	/*************************************************************************
	All Game objects have the same initial component connections, if those components exist.
	For instance, typically some entity in a game will need a render, physics, script, and gameplay at the least.
	Often time Input, Pathing, and Networking are very common as well.  So we go through the objects components,
	and find all of each type, and connect them appropriately to all comps of another specific type.  This graph
	is broken down as such:
	
	Graphics <- Physics ( <- = Reads from)
	Graphics <- Input
	
	Physics <- Input
	Physics <- Networking
	Physics -> Script
	Physics -> Graphics
	 
	Script <- Input
	Script -> Input

	Input -> Networking
	Input <- Networking

	This connectivity forms an implicit bi-directional graph but merely allows easy instant access to components
	without having to do lookups and casts at run-time.  Now it is simply the cost of casting, which can be
	done w/o dynamic_cast 
	*************************************************************************/

	//Go through each component....
	std::for_each(begin(mComponents), end(mComponents), [&](std::pair<unsigned int, ComponentDisciple* > comp) 
	{
		//.. and determine its type.
		switch(comp.second->family())
		{
			case Graphics:
				{
					//Physics - Read
					findAndAddComponentsOfFamily(this, comp.second, Physics, Read);
					//Input - Read			
					findAndAddComponentsOfFamily(this, comp.second, Input, Read);
				}
				break;

			case Physics:
				{
					//Input - Read
					findAndAddComponentsOfFamily(this, comp.second, Input, Read);
					//Script - Write
					findAndAddComponentsOfFamily(this, comp.second, Script, Write);
					//Graphics
					findAndAddComponentsOfFamily(this, comp.second, Graphics, Write);
					//Networking - Read			
					findAndAddComponentsOfFamily(this, comp.second, Networking, Bidirectional);
				}
				break;

			case Script:
				{
					//Input - Bidirectional
					findAndAddComponentsOfFamily(this, comp.second, Input, Bidirectional);
				}
				break;

			case Input:
				{
					//Networking - Bidirectional			
					findAndAddComponentsOfFamily(this, comp.second, Networking, Bidirectional);
				}
				break;

			case Gameplay:
				{
					//Script - Bidirectional
					findAndAddComponentsOfFamily(this, comp.second, Script, Bidirectional);
					//Input - Bidirectional
					findAndAddComponentsOfFamily(this, comp.second, Input, Bidirectional);
				}
				break;

			default:
				break;
		};
	});
}
ComponentDisciple*	GameObject::GetGraphicsComp(std::string name, std::string type)
{
	return NULL;
}


ComponentDisciple*	GameObject::GetPhysicsComp(std::string name, std::string type)
{
	return NULL;
}


#ifdef USE_LUA
LUA_START_DEFINE_CLASS( GameObject )
	LUA_NAME_METHOD( GameObject, "CreatePhysicsComponent", LCreatePhysicsComponent )
	LUA_NAME_METHOD( GameObject, "CreateRenderComponent", LCreateRenderComponent )
	LUA_NAME_METHOD( GameObject, "GetPhysicsComp", LGetPhysicsComp )
	LUA_NAME_METHOD( GameObject, "GetRenderComp ", LGetRenderComp )
	LUA_NAME_METHOD( GameObject, "SetScriptObjRef", LSetScriptObjRef )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( GameObject, LCreatePhysicsComponent )
{
	WorldScene* world = WorldScene::GetSingletonPtr();

	std::shared_ptr<RigidBodyDescriptor> rbDesc = std::make_shared<RigidBodyDescriptor>();

	auto obj = std::make_shared<GameObject>(*this);
	std::shared_ptr<ComponentDisciple> newcomp = world->mObjFactory->componentFactory()->Create(obj, rbDesc);

	Lunar<RigidBodyComponent>::push( ls, (RigidBodyComponent*)newcomp.get() );
	return 1;
}


LUA_DEFINE_METHOD( GameObject, LCreateRenderComponent )
{
	WorldScene* world = WorldScene::GetSingletonPtr();

	std::shared_ptr<ModelDescriptor> modelDesc = std::make_shared<ModelDescriptor>();

	auto obj = std::make_shared<GameObject>(*this);
	std::shared_ptr<ComponentDisciple> newcomp = world->mObjFactory->componentFactory()->Create(obj, modelDesc);

	Lunar<ModelComponent>::push( ls, (ModelComponent*)newcomp.get() );
	return 1;	
}


LUA_DEFINE_METHOD( GameObject, LGetPhysicsComp )
{	
	Lunar<PhysicsComponent>::push( ls, (PhysicsComponent*)GetPhysicsComp().get() );
	return 0;
}


LUA_DEFINE_METHOD( GameObject, LGetRenderComp )
{
	Lunar<RenderComponent>::push( ls, (RenderComponent*)GetGraphicsComp().get() );	
	return 0;
}

LUA_DEFINE_METHOD( GameObject, LSetScriptObjRef )
{
	// Grab a ref to the lua class if one was created
	if( lua_istable( ls, -1) )
	{
		mObjectRef = luaL_ref( ls, LUA_REGISTRYINDEX );
	}
	return 0;
}
#endif //if using Lua
