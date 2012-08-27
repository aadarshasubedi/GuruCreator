

#include "WorldScene.h"
#include "Game_Renderer.h"
#include "GameObjectDescriptor.h"
#include "PhysicsComponent.h"

#include <map>
#ifdef USE_LUA
#include "LunarHelper.h"
#endif
#ifdef USE_BOX2D
#include "Box2D.h"
#endif


WorldScene::WorldScene()
{
#ifdef USE_BOX2D
	mPhysicsWorld_Box2d = NULL;
#endif //if using box2d
	//Create the object factory.
	mObjFactory = new GameObjectFactory;
	mObjects = &(mObjFactory->mObjects);
}

WorldScene::~WorldScene()
{		
#ifdef USE_BOX2D
	delete mPhysicsWorld_Box2d;	
#endif //if using box2d

	//delete mObjFactory;
}

void WorldScene::initializePhysicsScene()
{	
	//Grab the data from a physicsWorld file.
    FILE* file;
	errno_t fileerror=0;

#ifdef WIN32		
	fileerror = fopen_s( &file, "resource/physicsWorld.txt", "r" );
#else
#endif

	if( fileerror )
	{
		return;
	}
	
#ifdef USE_BOX2D
	//Physics values being set during the read operation.
	b2AABB box;
	b2Vec2 gravity;
	unsigned int doSleep = 0;

	char buf[1024];

	while( !feof(file) )
    {
        //Data file exists, read from it.
        int error = fscanf( file, "%s", buf);

        if( strcmp( buf, "Bounds>" ) == 0 )
        {
            //Grab next two values.
            error = fscanf( file, "%f %f %f %f", &(box.lowerBound.x), &(box.lowerBound.y), &(box.upperBound.x), &(box.upperBound.y) );
        }
        else if( strcmp( buf, "Gravity>" ) == 0 )
        {
            error = fscanf( file, "%f %f", &(gravity.x), &(gravity.y) );
        }
        else if( strcmp( buf, "Sleep>" ) == 0 )
        {
            error = fscanf( file, "%u", &doSleep );
        }
        else
        {
        }
    }

	fclose(file);
	
	mPhysicsWorld_Box2d = new b2World(box, gravity, (doSleep) ? true : false);
	mPhysicsWorld_Box2d->SetContactListener( this );
#endif //using box2d world.

#ifdef WIN32		
	fileerror = fclose(file);
#else
#endif
}


void WorldScene::updatePhysics( float dt ) //parameter: 'dt' for the time step on the simulation
{
	mContactList.clear();

#ifdef USE_BOX2D
	mPhysicsWorld_Box2d->Step( dt, 20 );	
#else
	static float accum = 0.0f;
	accum += dt;
#endif
}


//std::shared_ptr<GameObject> WorldScene::CreateGameObject(const GameObjectDescriptor& description)
//{
//	return mObjFactory->CreateGameObject(description);
//}


GameObject* WorldScene::CreateGameObject(GameObjectDescriptor description)
{
	return mObjFactory->CreateGameObject(description);	
}


//Saving and loading functions to have various environments.
bool WorldScene::SaveSceneFile( const char*  )
{
	return true;
}

bool WorldScene::LoadSceneFile( const char*  )
{
	return true;
}


void WorldScene::init()
{
	mCcwSpin = true;
	mPauseAnimate = false;

	initializePhysicsScene();
}

void WorldScene::render()
{
}

#ifdef USE_BOX2D
void WorldScene::Add(const b2ContactPoint* point)
{
	PhysicsComponent *body1 = (PhysicsComponent *)point->shape1->GetUserData();
	PhysicsComponent *body2 = (PhysicsComponent *)point->shape2->GetUserData();

	ContactData newContact;
	if( body1 != NULL )
		newContact.obj1 = body1->GetParentObj();
	if( body2 != NULL )
		newContact.obj2 = body2->GetParentObj();

	if( !((GameObject*)newContact.obj1.get()) || !((GameObject*)newContact.obj2.get()) )
	{
		//There was an issue with one of the game objects.  Send their states to a log file for debugging.
		return;
	}

	newContact.position = CVector3(point->position.x, point->position.y, 0.0f);
	newContact.normal = CVector3(point->normal.x, point->normal.y, 0.0f);
	newContact.type = 1;
	mContactList.push_back( newContact );

	return;
}

void WorldScene::Persist(const b2ContactPoint* point )
{
	PhysicsComponent *body1 = (PhysicsComponent *)point->shape1->GetUserData();
	PhysicsComponent *body2 = (PhysicsComponent *)point->shape2->GetUserData();

	ContactData newContact;
	if( body1 != NULL )
		newContact.obj1 = body1->GetParentObj();
	if( body2 != NULL )
		newContact.obj2 = body2->GetParentObj();

	if( !newContact.obj1 || !newContact.obj2 )
	{
		//There was an issue with one of the game objects.  Send their states to a log file for debugging.
		return;
	}
	
	newContact.position = CVector3(point->position.x, point->position.y, 0.0f);
	newContact.normal = CVector3(point->normal.x, point->normal.y, 0.0f);
	newContact.type = 2;
	mContactList.push_back( newContact );

	return;
}

void WorldScene::Remove(const b2ContactPoint* point )
{
	PhysicsComponent *body1 = (PhysicsComponent *)point->shape1->GetUserData();
	PhysicsComponent *body2 = (PhysicsComponent *)point->shape2->GetUserData();

	ContactData newContact;
	if( body1 != NULL )
		newContact.obj1 = body1->GetParentObj();
	if( body2 != NULL )
		newContact.obj2 = body2->GetParentObj();

	if( !newContact.obj1 || !newContact.obj2 )
	{
		//There was an issue with one of the game objects.  Send their states to a log file for debugging.
		return;
	}
	
	newContact.position = CVector3(point->position.x, point->position.y, 0.0f);
	newContact.normal = CVector3(point->normal.x, point->normal.y, 0.0f);
	newContact.type = 3;
	mContactList.push_back( newContact );

	return;
}

void WorldScene::Result(const b2ContactResult* point)
{
	PhysicsComponent *body1 = (PhysicsComponent *)point->shape1->GetUserData();
	PhysicsComponent *body2 = (PhysicsComponent *)point->shape2->GetUserData();

	ContactData newContact;
	if( body1 != NULL )
		newContact.obj1 = body1->GetParentObj();
	if( body2 != NULL )
		newContact.obj2 = body2->GetParentObj();

	if( !newContact.obj1 || !newContact.obj2 )
	{
		//There was an issue with one of the game objects.  Send their states to a log file for debugging.
		return;
	}
	
	newContact.position = CVector3(point->position.x, point->position.y, 0.0f);
	newContact.normal = CVector3(point->normal.x, point->normal.y, 0.0f);
	newContact.type = 0;
	mContactList.push_back( newContact );

	return;
}
#endif //using box2D


#ifdef USE_LUA
//@TODO: Rewrite these with the new system implementation.
LUA_START_DEFINE_CLASS( WorldScene )

	LUA_NAME_METHOD( WorldScene, "CreateGameObject", LCreateGameObject )
	LUA_NAME_METHOD( WorldScene, "DeleteGameObject", LDeleteGameObject )
//	LUA_NAME_METHOD( WorldScene, "GetPhysicsContacts", LGetPhysicsContacts )
	LUA_NAME_METHOD( WorldScene, "SaveSceneFile", LSaveSceneFile )
	LUA_NAME_METHOD( WorldScene, "LoadSceneFile", LLoadSceneFile )
	LUA_NAME_METHOD( WorldScene, "SetClearColor", LSetClearColor )

	LUA_END_DEFINE_CLASS


	static int counter = 0;
LUA_DEFINE_METHOD( WorldScene, LCreateGameObject )
{
	//@todo, use the object factory instead
	//Creates a blank-slate game-object if there's nothing on the stack.  
	//If there are items on the stack, it will grab them and convert them into 
	// descriptors to call on the factory.
	GameObjectDescriptor desc;
	char dest[256];
	desc.mName = "Object_";
	desc.mName += std::string(_itoa(counter++, dest, 10));
	mObjFactory->CreateGameObject(desc);
	GameObjectPtr obj = mObjFactory->GetLastObjectCreated();
	Lunar<GameObject>::push( ls, obj.get() );
	return 1;
}

LUA_DEFINE_METHOD( WorldScene, LDeleteGameObject )
{
	//@todo, use the object factory instead
	GameObjectPtr obj = std::make_shared<GameObject>(*Lunar<GameObject>::check( ls, 1));
	mObjFactory->DeleteGameObject( obj );
	return 0;
}

//LUA_DEFINE_METHOD( WorldScene, LGetPhysicsContacts )
//{
//	for_each(begin(mContactList), end(mContactList), [](ContactData data)
//		
//	//for( unsigned i=0; i<mContactList.size(); ++i )
//	{
//		lua_newtable(ls);
//
//		//Game Object 1 as a reference to the lua table representing it
//		{
//			lua_pushstring(ls, "obj1");
//			
//			int objRef = mContactList[i].obj1->GetScriptObjRef();
//			lua_rawgeti( ls, LUA_REGISTRYINDEX, objRef );
//
//			lua_settable(ls, -3);
//		}
//
//		//Game Object 2 as a reference to the lua table representing it
//		{
//			lua_pushstring(ls, "obj2");
//			int objRef = mContactList[i].obj2->GetScriptObjRef();
//			lua_rawgeti( ls, LUA_REGISTRYINDEX, objRef );
//			lua_settable(ls, -3);
//		}
//
//		//Position as a vector table
//		{
//			lua_pushstring(ls, "position");
//			lua_newtable(ls);
//				lua_pushstring(ls, "x");
//				lua_pushnumber(ls, mContactList[i].position.x);
//				lua_settable(ls, -3);
//			
//				lua_pushstring(ls, "y");
//				lua_pushnumber(ls, mContactList[i].position.y);
//				lua_settable(ls, -3);
//			lua_settable(ls, -3);
//		}
//
//		//Normal as a vector table
//		{
//			lua_pushstring(ls, "normal");
//			lua_newtable(ls);
//				lua_pushstring(ls, "x");
//				lua_pushnumber(ls, mContactList[i].normal.x);
//				lua_settable(ls, -3);
//			
//				lua_pushstring(ls, "y");
//				lua_pushnumber(ls, mContactList[i].normal.y);
//				lua_settable(ls, -3);
//			lua_settable(ls, -3);
//		}
//
//		//type
//		{
//			lua_pushstring(ls, "type");
//			switch(mContactList[i].type)
//			{
//			case 1:
//				lua_pushstring( ls, "enter" );
//				break;
//			case 2:
//				lua_pushstring( ls, "stay" );
//				break;
//			case 3:
//				lua_pushstring( ls, "exit" );
//				break;
//			default:
//				lua_pushinteger(ls, mContactList[i].type);
//			}
//			lua_settable(ls, -3);
//		}
//	});
//	return mContactList.size();
//}

LUA_DEFINE_METHOD( WorldScene, LSaveSceneFile )
{
	//ls is automatic parameter
	if ( !ls ) { return 0; }

	//Grab the filename.
	const char* filename = lua_tostring(ls,1);

	SaveSceneFile(filename);

	return 0;
}

LUA_DEFINE_METHOD( WorldScene, LLoadSceneFile )
{
	//ls is automatic parameter
	if ( !ls ) { return 0; }

	//Grab the filename.
	const char* filename = lua_tostring(ls,1);

	LoadSceneFile(filename);

	return 0;
}

LUA_DEFINE_METHOD( WorldScene, LSetClearColor )
{
	//ls is automatic parameter
	if ( !ls ) { return 0; }
	
	float r = (float)lua_tonumber(ls, 1 );
	float g = (float)lua_tonumber(ls, 2 );
	float b = (float)lua_tonumber(ls, 3 );
	float a = (float)lua_tonumber(ls, 4 );
	glClearColor( r, g, b, a );

	return 0;
}
#endif //using Lua
