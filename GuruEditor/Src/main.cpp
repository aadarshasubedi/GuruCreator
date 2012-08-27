/*
Author:		Dan Strycharske
Date:       06.05.09
File:       main.cpp
Brief:      The alpha and omega.
*/

#ifdef WIN32
#include <windows.h>
#endif


#include "Game_Engine.h"

int main ( int , char**  )
{
#ifdef USE_BOX2D
	bool doSleep = true;
	Game_Engine* TEngine = new Game_Engine(doSleep);
#else
	Game_Engine* TEngine = new Game_Engine();
#endif	

    TEngine->EngineInitialize();
    TEngine->EngineGo();

	delete TEngine;

    // all is well ;)
    printf("Exited cleanly\n");
	return 0;
}
