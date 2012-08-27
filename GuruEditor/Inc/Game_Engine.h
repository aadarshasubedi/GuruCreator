/*
Author:		Dan Strycharske
Date:       06.05.09
File:       Engine.h
Brief:      A pure virtual engine class that will be task based.
*/

#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H


//
//A game engine class that holds all the necessary modules and controls their interactions and exposes them to the global system.  In this way the render system can just check out the physics
//  system data more easily then through complex messaging or massive amounts of indirection.
//
//A gameplay system class that controls the game timer, points collected, % of area complete, HUD, and other abstract gameplay data and logic that needs to be controlled, including the rules 
//  of play and determing failure, death, success, time out, etc.
//

#include "Engine.h"
#include "WorldScene.h"

#ifdef USE_BOX2D
#include "Box2D.h"
#endif
#ifdef USE_LUA
#include "LunarHelper.h"
#endif


class GamePicker;


class Game_Engine : public Engine
{

public:
	Game_Engine();
#ifdef USE_BOX2D
	Game_Engine( bool doSleep, const b2AABB& box = b2AABB(), const b2Vec2& gravity = b2Vec2(0.0f, -10.0f) );
#endif

	~Game_Engine();

	Game_Engine( const Game_Engine& eng );
	void operator=( const Game_Engine& eng );

	//@TODO: Need to make private with accessors, create functions, and no more.
	//My world scene, holding all the components, simulation details, and interfaces for simulation results.
	WorldScene* mWorldScene;

	GamePicker* mPicker;
	//////////////////////////////////////////////////////////////////////////////////////////////////////

	//The picker should hold this, and does, but for easy testing I'm using this value.  These are a bunch
	// of global values for various things needed by the picker.
	unsigned int mNavigationMode;

	//Needed by camera.
	float mCamPitchSpd;
	float mCamYawSpd;
	float mCamRollSpd;

#ifdef USE_BOX2D
	b2World mPhysicsWorld_Box2d;
#endif

//Coming from the base Engine class.
public:
    //Initialize engine machines.

	virtual void ParseInputDataFile();
	virtual void InitializeWindow() { Engine::InitializeWindow(); }
    virtual void InitializeThinker();
    virtual void InitializeRenderer() { Engine::InitializeRenderer(); }
    virtual void InitializePhysics();
    virtual void InitializeInput() { Engine::InitializeInput(); }
    virtual void InitializeExtra();

    //Engine machine update routines.
    virtual void UpdateWindow( float time, float timestep ) { Engine::UpdateWindow(time, timestep); }
	virtual void UpdateThinker( float time, float timestep ) { Engine::UpdateThinker(time, timestep); }
    virtual void UpdateRenderer( float time, float timestep );
    virtual void UpdatePhysics( float time, float timestep );
    virtual void UpdateInput( float time, float timestep ) { Engine::UpdateInput(time, timestep); }

    //Engine Window functions
    virtual void WinActivate() { Engine::WinActivate(); }
    virtual void WinDeactivate() { Engine::WinDeactivate(); }

    // Calculate
    virtual void CalculateFPS() { Engine::CalculateFPS(); }
    // Display
    virtual void DisplayFPS() { Engine::DisplayFPS(); }
    // Lock framerate
    virtual void LockFPS( int WantThisFPS ) { Engine::LockFPS(WantThisFPS); }

	//Do some extra stuff with the input checking.
	virtual void Input( float t, float dt );
			
    // Rename the window.
	virtual void ChangeWinName( const char* WantThisName ) { Engine::ChangeWinName(WantThisName); }

	
#ifdef USE_LUA
protected:
	//The Lua global registration function.
	virtual void RegisterGlobals();
	virtual void RegisterClasses();
#endif //if using Lua
	//void RegisterEngineGlobals();
	//void RegisterEngineClasses();
};

#endif
