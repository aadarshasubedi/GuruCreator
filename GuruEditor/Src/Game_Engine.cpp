/*
Author:		Dan Strycharske
Date:       06.05.09
File:       Engine.h
Brief:      A pure virtual engine class that will be task based.
*/

#pragma warning( disable : 4481)

//
#include "Game_Engine.h"
#include "ParticleEngine.h"
#include "Game_Renderer.h"
#include "GameObject.h"
#include "GameObjectDescriptor.h"
#include "RigidBodyDescriptor.h"
#include "RigidBodyComponent.h"
#include "ModelDescriptor.h"
#include "ModelUtility.h"
#include "RenderComponent.h"
#include "Engine.h"
#include "DScriptGuru.h"
#include "DScriptLibraries.h"
#include "WorldScene.h"
#include "Game_Picker.h"
#ifdef USE_LUA
#include "LunarHelper.h"
#endif


/*************************************************************************
 
*************************************************************************/


/*************************************************************************
 
*************************************************************************/


Game_Engine::Game_Engine( 
#ifdef USE_BOX2D
	bool doSleep, const b2AABB& box, const b2Vec2& gravity) : mPhysicsWorld_Box2d( box, gravity, doSleep )
#else	
	)
#endif
{
	//@TODO: All of this needs to not be here, but either in an Init function or divyed out to another class,
	// where that will be initialized here.
	mWorldScene = new WorldScene();
	mNavigationMode = 0; //0 is regular free-cam, 1 is pick mode, 
	mPicker = new GamePicker;
	//Setting global values.  These should go into some sort of "EditorOptions" class, which dictates
	// things like selection mode, camera movement/rotate speed, visualize grid, etc.
	mCamPitchSpd = 8.0f;
	mCamRollSpd = 8.0f;
	mCamYawSpd = 8.0f;
}


Game_Engine::~Game_Engine( )
{
	delete mWorldScene;
	delete mPicker;
}


void Game_Engine::ParseInputDataFile()
{
	const char * filename = "./../GuruResource/EngineData.txt";
	FILE* file;

#ifdef WIN32
	errno_t fileerror = fopen_s( &file, filename, "rt" );  //Make sure it's null terminated, got me for a while.
#else
	file = new FILE;
	file = fopen( "./../GuruResource/EngineData.txt", "rt" );
#endif

	if( file == 0 || fileerror < 0)
	{
		this->EngineSeizeUp();
	}

	char buf[256] = {};
	int error = 0;
	unsigned type = SDL_GL_MODULE;

	CRenderModule::RMOptions options;

	while( !feof(file) )
	{
		//Data file exists, read from it.		
		error = fscanf( file, "%s", buf);

		if( strcmp( buf, "Resolution>" ) == 0 )
		{
			//Grab next two values.
			error = fscanf( file, "%i %i", &options.mWidth, &options.mHeight );
		}
		else if( strcmp( buf, "Backbuffer>" ) == 0 )
		{
			error = fscanf( file, "%u %u %u", &mBackColor[0], &mBackColor[1], &mBackColor[2] );
		}
		else if( strcmp( buf, "FPS>" ) == 0 )
		{
			error = fscanf( file, "%u", &mFPSDesired );
		}
		else if( strcmp( buf, "LogFile>" ) == 0 )
		{
			error = fscanf( file, "%s", mLogFilename );
		}
		else if( strcmp( buf, "Renderer>" ) == 0 )
		{
			error = fscanf( file, "%u", &type );
		}
		else if( strcmp( buf, "TestDir>" ) == 0 )
		{
			error = fscanf( file, "%s", mResourceDir );
		}
		else if( strcmp( buf, "TestFile>" ) == 0 )
		{
			error = fscanf( file, "%s", mTestFile );
		}
		else
		{
		}
	}

	switch( type )
	{
	case SDL_MODULE:
		{
			break;
		}
	case SDL_GL_MODULE:
		{
			options.mRenderOptions = 0;
			mRenderModules.insert( std::make_pair( 0, new Game_Renderer(options) ) );
			break;
		}

	default:
		{
			options.mRenderOptions = 0;
			mRenderModules.insert( std::make_pair( 0, new Game_Renderer(options) ) );
			break;
		}
	}

	mCurRenderer = 0;

	//Clean up the file I opened.
	fclose(file);
}

void Game_Engine::InitializeThinker()
{ 
	Engine::InitializeThinker(); 

	//RegisterObjectLibrary(mScriptGuru->GetMainLuaState());
}


void Game_Engine::InitializeExtra()
{ 	
	Engine::InitializeExtra(); 

	mWorldScene->init();

#ifdef USE_LUA

	RegisterClasses();
	RegisterGlobals();

	//DScriptGuru::GetSingletonPtr()->CreateImmediateDisciple( "./resource/FirstWizardScript.lua" );
#endif //if using Lua
}


void Game_Engine::InitializePhysics()
{
	GameObjectDescriptor sphereDesc;
	auto sphereRBDesc = new RigidBodyDescriptor;
	auto sphereModDesc = new ModelDescriptor(CreateSphere(NULL, 10, 8, 6));
	sphereDesc.mPosition = CVector3(0,0,0);
	sphereDesc.mOrientation = DQuaternion();
	sphereDesc.AddComponentDescriptor(sphereRBDesc);
	sphereDesc.AddComponentDescriptor(sphereModDesc);

	auto sphereObj = mWorldScene->CreateGameObject(sphereDesc);

	//Create the first object.
	//GameObjectDescriptor sphereDesc;
	//auto sphereRbDesc = std::make_shared<RigidBodyDescriptor>();	
	//auto sphereModDesc = std::make_shared<ModelDescriptor>(CreateSphere(NULL, 20, 8, 6));
	//
	//sphereDesc.mPosition = CVector3(45,0,0);
	//sphereDesc.mOrientation = DQuaternion();
	//
	//sphereDesc.AddComponentDescriptor(sphereRbDesc);
	//sphereDesc.AddComponentDescriptor(sphereModDesc);

	//mWorldScene->CreateGameObject(sphereDesc);
	//auto sphere = mWorldScene->mObjFactory->GetLastObjectCreated();

	////Create the second object.
	//GameObjectDescriptor boxDesc;
	//auto boxRbDesc = std::make_shared<RigidBodyDescriptor>();
	//auto boxModDesc = CreateBox( NULL, 20, 20, 20);
	//boxDesc.mPosition = CVector3(0,0,0);

	//boxDesc.AddComponentDescriptor(boxRbDesc);
	//boxDesc.AddComponentDescriptor(std::make_shared<ModelDescriptor>(boxModDesc));

	//mWorldScene->CreateGameObject(boxDesc);
	//auto box = mWorldScene->mObjFactory->GetLastObjectCreated();
	//
	////Create the third object.
	//GameObjectDescriptor cylDesc;
	//auto cylRbDesc = std::make_shared<RigidBodyDescriptor>();
	//auto cylModDesc = CreateCylinder(NULL, 20, 20, 8);
	//cylDesc.mPosition = CVector3(-45,0,0);

	//cylDesc.AddComponentDescriptor(cylRbDesc);
	//cylDesc.AddComponentDescriptor(std::make_shared<ModelDescriptor>(cylModDesc));

	//mWorldScene->CreateGameObject(cylDesc);
	//auto cyl = mWorldScene->mObjFactory->GetLastObjectCreated();
}


void Game_Engine::UpdateRenderer( float , float  )
{
	Module()->BeginRender();
		mWorldScene->render();
		Module()->render();
	Module()->EndRender();
}


void Game_Engine::UpdatePhysics( float , float timestep )
{
	mWorldScene->updatePhysics( timestep );
}


void Game_Engine::Input(float , float dt)
{
	float spd = 3.0f;
		
	Camera* camera = Module()->GetCamera();

	mInputGuru->PollAllInputDevices();

	//if( mNavigationMode == 0 )
	{
		if( mInputGuru->IsKeyHeld( SDLK_w) ) 
		{
			if( camera->PType() )
			{
				camera->TranslateForward( spd );
			}
			else
			{
				camera->TranslateUp( spd );
			}
		}
		if( mInputGuru->IsKeyHeld( SDLK_s) )
		{
			if( camera->PType() )
			{
				camera->TranslateForward( -spd );
			}
			else
			{
				camera->TranslateUp( -spd );
			}
		}
		if( mInputGuru->IsKeyHeld( SDLK_a) )
		{
			camera->TranslateRight( -spd );
		}
		if( mInputGuru->IsKeyHeld( SDLK_d) )
		{
			camera->TranslateRight( spd );
		}
		if( mInputGuru->IsKeyHeld( SDLK_SPACE ) )
		{
			camera->TranslateUp( spd );
		}
		if( mInputGuru->IsKeyHeld( SDLK_LSHIFT ) )
		{
			camera->TranslateUp( -spd );
		}

		//Rotating camera, normal motions.
		if( mInputGuru->MouseHeld(SDL_BUTTON_LEFT) && mInputGuru->MouseMoving() )
		{
			int changeX = mInputGuru->GetMouseDY();
			int changeY = mInputGuru->GetMouseDX();
			camera->ChangePitch( (float)changeX * dt * mCamPitchSpd );
			camera->ChangeHeading( (float)changeY * dt * mCamYawSpd);
		}
	}

	//////////////////////////////////////////////////////////////////////////////
	static bool objectPicked = false;
	static Entity* pickedObject = NULL;
	if( mNavigationMode == 1 && mInputGuru->MouseClick(SDL_BUTTON_RIGHT) )
	{		
		//Project a ray cast using the GPicker.
		pickedObject = mPicker->PickSceneGraphSingleObject(mInputGuru->curx, mInputGuru->cury);
		objectPicked = true;
	}
	else if( mNavigationMode == 1 && objectPicked && pickedObject != NULL && mInputGuru->MouseHeld(SDL_BUTTON_RIGHT) && mInputGuru->MouseMoving())
	{
		pickedObject->RotateX(20.0f);
	}
	
	if( mInputGuru->MouseRelease(SDL_BUTTON_RIGHT) )
	{
		objectPicked = false;
		pickedObject = NULL;
	}

	//////////////////////////////////////////////////////////////////////////////
	if( mInputGuru->IsKeyPressed(SDLK_KP0) )
	{
		mNavigationMode = 0;
		objectPicked = false;
		pickedObject = NULL;
	}
	else if(mInputGuru->IsKeyPressed(SDLK_KP1) )
	{
		mNavigationMode = 1;
		objectPicked = false;
		pickedObject = NULL;
	}

	if( mInputGuru->IsKeyPressed(SDLK_i) )
	{
		((Game_Renderer*)Module())->ToggleWireFrame();
	}
	if( mInputGuru->IsKeyPressed(SDLK_ESCAPE) )
	{
		mHasQuit = true;
	}
	//////////////////////////////////////////////////////////////////////////////
}


#ifdef USE_LUA
void Game_Engine::RegisterClasses()
{
	lua_State* ls = DScriptGuru::GetSingletonPtr()->GetMainLuaState();
	
	//@TODO: Reregister the needed classes with the appropriate setup.
	//Register any other classes that the base engine does not know about.
	Lunar<WorldScene>::Register( ls );
	Lunar<RigidBodyComponent>::Register(ls);
	Lunar<ModelComponent>::Register(ls);
	Lunar<GameObject>::Register(ls);

	//So we can reference box2d's shapes in Lua and in C.
#ifdef USE_BOX2D
	LunarLite<b2Shape>::Register(ls, "b2Shape" );
	LunarLite<b2Vec2>::Register(ls, "b2Vec2" );
#endif		
	LunarLite<ComponentDisciple>::Register(ls, "Component");
	//LunarLite<GameObject>::Register(ls, "GameObject");
}


void Game_Engine::RegisterGlobals()
{
	//Register other global classes that the lower level base engine does not know about.
	//@TODO: Reregister the component manager since it's not global, it's in the game factory so bind that.
	//mScriptGuru->RegisterGlobal(mCompManager, "RCompManager");
	mScriptGuru->RegisterGlobal( mWorldScene, "WorldScene" );
}
#endif //if using Lua
