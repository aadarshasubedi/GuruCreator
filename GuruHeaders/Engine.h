/*
Author:		Dan Strycharske
Date:       06.05.09
File:       Engine.h
Brief:      A pure virtual engine class that will be task based.

			Engine contains these things:

			Window + Attributes
			All objects
			Timing Machine
			Render Machine
			Think Machine
			Input Machine
			Physics Machine
			Update Machine
			Task Machine

			Anything that needs to be done by the Engine is broken down into a task.
			A Task is something that either the CPU or GPU must work on.
			The tasks will be virtual themselves so that if ported to a different machine,
			the various tasks can be handled differently depending on hardware limitations.

			These tasks will be allocated by priority within the remaining time left in the frame.
			Any task waiting over multiple frames increases in priority automatically.
*/

#ifndef ENGINE_H_INCLUDED
#define ENGINE_H_INCLUDED


#ifdef PURE_WINDOWS
	#include "glew.h"
#else 
	#include "gl_inc.h"
#endif

#include <map>  //For std::map, stores the renderers.
#include <algorithm>
#include <stdio.h>

#include "InputGuru.h"
#include "CRenderModule.h"
#include "Camera.h"
#include "DScriptGuru.h"
#include "Logger.h"
#include "DObject.h"
#include "AssetManager.h"
#include "RenderComponent.h"
#include "PhysicsComponent.h"
#include "ModelComponent.h"

#ifdef USE_LUA
#include "LunarHelper.h"
#endif


const int SDL_MODULE = 1;
const int SDL_GL_MODULE = 2;


namespace Dan
{
    //Declarations.
    char* my_itoa( int, int );
}


class Engine : public DSingleton<Engine>
{
public:
	//Initializers and start up functions for tasks and systems.
	Engine();
	virtual ~Engine();

	void EngineInitialize();

	//Main loop starting function.  Continues until either EngineShutDown or EngineSeizeUp is called.
	void EngineGo();

	//This will be the engine shutdown routine resulting from an error.
	void EngineSeizeUp();
	//Elegant engine close function, upon menu, game or window close calls.
	void EngineShutDown();
		
	//////////////////////////////////////////////////////////
	//VIRTUAL FUNCTIONS //////////////////////////////////////
	//////////////////////////////////////////////////////////
	//Initialize engine machines.
	virtual void InitializeWindow();
	virtual void InitializeThinker();
	virtual void InitializeRenderer();
	virtual void InitializePhysics();
	virtual void InitializeInput();
	virtual void InitializeExtra();

	//Engine machine update routines.
	virtual void UpdateWindow( float time, float timestep );
	virtual void UpdateThinker( float time, float timestep );
	virtual void UpdateRenderer( float time, float timestep );
	virtual void UpdatePhysics( float time, float timestep );
	virtual void UpdateInput( float time, float timestep );

	//Engine Window functions
	virtual void WinActivate();
	virtual void WinDeactivate();

	// Calculate
	virtual void CalculateFPS();
	// Display
	virtual void DisplayFPS();
	// Lock frame rate
	virtual void LockFPS( int WantThisFPS );

	//Window
	// Rename
	virtual void ChangeWinName( const char* WantThisName );

	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////

	// Get name
	const char* GetWinName() const;

	//Quick access to the current rendering module.
	int CurrentRenderModule();
	CRenderModule* Module();

	const bool& HasQuit() const { return mHasQuit; }

protected:
    //Timer values...  for various tick and frame rates.
    // Last frames tick value.
    unsigned mLastTick;
    // Stores the current frame rate.
    unsigned mFrameRate;
    // Used to calculate final frame rate.
    unsigned mTickTime;
    // Frame counter... used to calculate frame rate.
    unsigned mFrameCounter;
    // Desired frame rate for locked rates.
    unsigned mFPSDesired;

    //Check if user has quit.
    bool mHasQuit;

    //Window dimensions.
    unsigned mWinWidth;
    unsigned mWinHeight;
    // Focus
    bool mWinInFocus;
    // Resize
    bool mWinResize;
    // Name/Title
    char mWinName[256];

    char mResourceDir[256];
    char mTestFile[256];

    //SDL screen surface, used to determine render target.
    SDL_Surface* mScreenSurface;

    char        mSysToUpdate;

    char        mLogFilename[256];
    bool        mLogCapable;
    bool        mLogCurrent;

    int         MY_DEBUG;

    //////////////RENDER MODULES AND OTHER GRAPHICAL ATTRIBUTES///////////////////////
    std::map< unsigned, CRenderModule*> mRenderModules;
    int         mCurRenderer;
    unsigned    mBackColor[3];

    ///////////////INPUT GURU/////////////////////////////////////////////////////////
    DInputGuru*		mInputGuru;

    ///////////////Logging GURU/////////////////////////////////////////////////////////
	DLogGuru*		mLogGuru;

	///////////////Scripting GURU/////////////////////////////////////////////////////////
	DScriptGuru*	mScriptGuru;

public:
	AssetManager	mAssetManager;
	
protected:
    virtual void Think(float t, float dt);
    virtual void Render(float t, float dt);
    virtual void Simulate(float t, float dt);
    virtual void Input(float t, float dt);	
    
    void SetWinSize( const int& w, const int& h );

    //all engines will have these logging functions available to them.
    void InitializeLogger( const char* filepath, const char* rwProps );
    void WriteLogger( const char* LogMessage );

	virtual void ParseInputDataFile() = 0;

#ifdef USE_LUA
public:
	virtual void RegisterClasses() 
	{
		lua_State* ls = DScriptGuru::GetSingletonPtr()->GetMainLuaState();

		Lunar<Camera>::Register( ls);	
		Lunar<DScriptGuru>::Register( ls);	
		Lunar<DInputGuru>::Register( ls);	
		Lunar<RenderComponent>::Register(ls);
		Lunar<PhysicsComponent>::Register(ls);
	}

	virtual void RegisterGlobals() 
	{
		mScriptGuru->RegisterGlobal(Module()->GetCamera(), "Camera");
		mScriptGuru->RegisterGlobal(mScriptGuru, "DScriptGuru");
		mScriptGuru->RegisterGlobal(mInputGuru, "DInputGuru");
	}
#endif //if using Lua
};


#endif // ENGINE_H_INCLUDED


