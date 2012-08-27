/*
Author:		Dan Strycharske
Date:		06.05.09
File:		Engine.cpp
Brief:      Implementation of the base pure virtual class and it's non virtual functions.


*/


#include "Engine.h"
#include "Camera.h"
#include <stdio.h>


namespace Dan
{
    char* my_itoa( int val, int base )
    {
        static char buf[32] = {0};
        static const char* baseVals = "1234567890abcdef";

        //set up the loop iterator.  starts at 30 because indices into buf are 0-31, and first value is already set.
        unsigned i = 30;

        for( ; val && i; --i, val /= base )
        {
            buf[i] = baseVals[ val % base ];
        }

        //Depending on when loop cut out, only return piece of buffer that loop iterator i got to.
        return &buf[i + 1];
    }
}


using namespace Dan;


//Initializers and start up functions for tasks and systems.
Engine::Engine()
{
    //Intialize the name to default, and all other values to zero or identity.
    mLastTick = 0;
    mFrameRate = 0;
    mFrameCounter = 0;
    mTickTime = 0;
    mSysToUpdate = 0;

    mWinWidth = 100;
    mWinHeight = 100;

    mHasQuit = false;
    mWinInFocus = true;

    ChangeWinName( "DefaultName" );

    mScreenSurface = NULL;

    mLogCapable = false;
    mLogCurrent = false;

    MY_DEBUG = -1;

    mCurRenderer = -1;

	mScriptGuru = NULL;
	mInputGuru = NULL;
	mLogGuru = NULL;
}


Engine::~Engine()
{
	delete mScriptGuru;
	delete mInputGuru;
	delete mLogGuru;
	delete (Module()->manager());
	delete Module();
}


void Engine::EngineInitialize()
{
    //Register the SDL_Quit callback for the exit call.
    atexit( SDL_Quit );
	
    //Initialize SDL video system, check for errors
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        //This means there was an error.  Assert, break, or throw error.
        fprintf( stderr, "Unable to init SDL video: %s\n", SDL_GetError() );
        EngineSeizeUp( );
    }
	
    //Parse the input data file for all the systems and see what we get.
    ParseInputDataFile( );

    //Create the render objects and devices as necessary.
    InitializeRenderer();

    //Create appropriately sized window.
    InitializeWindow();

    //Start the logger.
    InitializeLogger( mLogFilename, "r+" );

    //Start the input updater
    InitializeInput();

    //Start the AI think machine.
    InitializeThinker();

    //Create the simulater
    InitializePhysics();

    InitializeExtra();
}


void Engine::EngineGo()
{
    //Grab first tick and start the ball rolling.
    float cur_time = 0.0f;
    float simulate_step = 1 / 60.0f;

    mLastTick = SDL_GetTicks();
    mHasQuit = false;

    while( !mHasQuit )
    {
        //Always update the input, regardless of whether it's actions are called.
        UpdateInput(cur_time, simulate_step);
        UpdateWindow(cur_time, simulate_step);
        UpdateThinker(cur_time, simulate_step);

        //These two are parsed out because the accumulator will dictate what truly gets passed to these.
        UpdatePhysics(cur_time, simulate_step);
        UpdateRenderer(cur_time, simulate_step);

        if( mWinInFocus )
        {
            //Run systems wanted at moment.
            Think(cur_time, simulate_step);
            Simulate(cur_time, simulate_step);
            Render(cur_time, simulate_step);
            Input(cur_time, simulate_step);
        }
        else
        {
            //Window is minimized so don't update anything.
        }
    }

    EngineShutDown();
}


void Engine::EngineShutDown()
{
    mRenderModules[mCurRenderer]->close();

    mHasQuit = true;
}


void Engine::EngineSeizeUp()
{
}


//Initialize engine machines.
void Engine::InitializeWindow()
{
    //Optimally, this would poll some sort of hardware specs to figure out the right display.
    Module()->initWindow();
}


void Engine::InitializeThinker()
{
	//Here I'll load in the main entry point script.  From here most other scripts will be generated.
	// If necessary, a script list will be held by the Script guru to update them all.  As of now, there's
	// only the one.
    mScriptGuru = new DScriptGuru();
}


void Engine::InitializeRenderer()
{
	Module()->SetAssetManager(new AssetManager);
    Module()->init();
}


void Engine::InitializePhysics()
{
}


void Engine::InitializeInput()
{
    mInputGuru = new DInputGuru();
}


void Engine::InitializeLogger( const char* filepath, const char* )
{
    mLogGuru = new DLogGuru( filepath );
}


void Engine::InitializeExtra()
{

#ifdef USE_LUA

	Engine::RegisterClasses();
	Engine::RegisterGlobals();

#endif

}


//Engine machine update routines.
void Engine::UpdateWindow( float , float  )
{
  //  // message processing loop
    SDL_Event theEvent;
    while (SDL_PollEvent(&theEvent))
    {
        switch (theEvent.type)
        {			
			case SDL_QUIT:
			{
				mHasQuit = true;
				break;
			}

			default:
			{
				break;
			}

        } // end switch

    } // end of message processing
}


void Engine::UpdateThinker( float , float timestep )
{
	mScriptGuru->UpdateDisciples( timestep );
}


void Engine::UpdateRenderer( float, float  )
{
}


void Engine::UpdatePhysics( float , float  )
{
}


void Engine::UpdateInput( float , float )
{
}


//Engine action functions
void Engine::Render(float , float)
{
    //Calculate the frame rate.
    CalculateFPS();

	CRenderModule* module = (*mRenderModules.find(mCurRenderer)).second;
    module->render();
}


void Engine::Think(float , float )
{

}


void Engine::Simulate(float , float )
{
}


void Engine::Input(float, float )
{   
}


void Engine::WriteLogger( const char* LogMessage )
{
    //Make sure log file exists and logger can be written to.
    if( mLogCapable && mLogCurrent )
    {
        mLogGuru->LogSingleMessage( LogMessage );
    }
}


void Engine::SetWinSize( const int& w, const int& h )
{
    mWinWidth = w;
    mWinHeight = h;

}


//Engine Window functions
void Engine::WinActivate()
{
}


void Engine::WinDeactivate()
{
}


//Framerate.
// Calculate
void Engine::CalculateFPS()
{
    //Increase frame counter.
    ++mFrameCounter;

    //See if the ticks tell us that one second has gone by.
    if( mTickTime >= 1000 )
    {
        //Frame counter is now the number of frames counter in this elapsed second.
        mFrameRate = mFrameCounter;
        mTickTime = 0;
        mFrameCounter = 0;
    }
}


// Display
void Engine::DisplayFPS()
{
    ChangeWinName( my_itoa( mFrameRate, 10 ) );
}


// Lock framerate
void Engine::LockFPS( int  )
{
}


//Window
// Rename
void Engine::ChangeWinName( const char*  )
{
}


// Get name
const char* Engine::GetWinName() const
{
    return mWinName;
}


int Engine::CurrentRenderModule()
{
    return mCurRenderer;
}


CRenderModule* Engine::Module()
{
    return mRenderModules[mCurRenderer];
}

