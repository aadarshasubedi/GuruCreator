#ifndef __WORLD_SCENE_H_
#define __WORLD_SCENE_H_


#include "DSingleton.h"
#include "GameObjectFactory.h"
#include "Utility.h"
#include <map>
#ifdef USE_LUA
	#include "LunarHelper.h"
#endif
#ifdef USE_BOX2D
	#include "Box2D.h"
#endif


class GameObjectDescriptor;


class WorldScene : public DSingleton<WorldScene>

//Only if using box2d does the WorldScene need to listen for the contacts.
#ifdef USE_BOX2D
	, public b2ContactListener 
#endif
/////////////////////////////////////////////////////
{
public:
	WorldScene();
	~WorldScene();

	void init();
	void initializePhysicsScene();

	void updatePhysics( float dt );

	void render();

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	typedef struct ContactData
	{
		ContactData() : obj1(), obj2() { }
		~ContactData() {}
		GameObjectPtr obj1;
		GameObjectPtr obj2;
		CVector3 position;
		CVector3 normal;
		int type;
	} ContactData;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	std::vector<ContactData> mContactList;

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//std::shared_ptr<GameObject> CreateGameObject(const GameObjectDescriptor& description);
	GameObject* CreateGameObject(GameObjectDescriptor description);

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	//Saving and loading functions to have various environments.
	bool SaveSceneFile( const char* filename );
	bool LoadSceneFile( const char* filename );

	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	bool mCcwSpin;
	bool mPauseAnimate;
	
	/************************************************************************/
	/*                                                                      */
	/************************************************************************/
	GameObjectFactory* mObjFactory;
	std::map<unsigned int, GameObjectPtr>* mObjects;

/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifdef USE_BOX2D
	void Render_Box2dWorld();

	//the single physics world that will exist.
	b2World* mPhysicsWorld_Box2d;

	//A test object for the physics.  will use one of my generic render functions for now so that I can see a dot of where it's going.
	b2Body*	mBody;
	b2Body* bodyTwo;

	b2Body* mGround;

	private:
		//From b2ContactListener - Contact notification functions
		void Add(const b2ContactPoint* point);
		void Persist(const b2ContactPoint* point);
		void Remove(const b2ContactPoint* point);
		void Result(const b2ContactResult* point);

#endif //if using box2d			
/************************************************************************/
/*                                                                      */
/************************************************************************/
#ifdef USE_LUA
	LUA_DECLARE_CLASS( WorldScene )
	LUA_DECLARE_METHOD( LCreateGameObject )
	LUA_DECLARE_METHOD( LDeleteGameObject )
	LUA_DECLARE_METHOD( LGetPhysicsContacts )
	LUA_DECLARE_METHOD( LSaveSceneFile )
	LUA_DECLARE_METHOD( LLoadSceneFile )
	LUA_DECLARE_METHOD( LSetClearColor )
#endif //if using Lua
///////////////////////////////////////////

};

#endif