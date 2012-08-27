/************************************************************************
Author:		Dan Strycharske
Date:       02.06.12
File:       GuruAPI.h
Brief:      
/************************************************************************/

#ifndef GuruAPI_H
#define GuruAPI_H


#pragma warning( disable : 4244 )


/*************************************************************************
 STANDARD LIBRARY INCLUDES
 *************************************************************************/
#ifdef PURE_WINDOWS
	#include <process.h>
#else
#endif

#ifdef __cplusplus
	#include <cstdlib>
#else
	#include <stdlib.h>
#endif
#include <cmath>
#include <cassert>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <list>
#include <map>
#include <algorithm>
#include <functional>


#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif


///*************************************************************************
// BOX2D INLCUDES
//*************************************************************************/
#ifdef USE_BOX2D
#include "Box2D.h"
#endif

#ifdef USE_LUA
#include "LunarHelper.h"
#endif


/*************************************************************************
 OPENGL INCLUDES
*************************************************************************/

#include "gl_inc.h"


/*************************************************************************
 SDL INCLUDES
*************************************************************************/
#include "SDL.h"
//#include "SDL_opengl.h"



/*************************************************************************
 MATH LIB INLCUDES
*************************************************************************/
#include "DQuaternion.h"
#include "Utility.h"
#include "VQS.h"
#include "Matrix.h"
#include "Vector.h"
#include "Primitives.h"


/*************************************************************************
 GURU ENGINE INCLUDES
 *************************************************************************/

/*************************************************************************
	GRAPHICS
*************************************************************************/
//Utilities
#include "DBitmap.h"

//System
#include "Helper.h"
#include "CRenderModule.h"
#include "Camera.h"


/*************************************************************************
	ENGINE	
*************************************************************************/
//System
#include "DSingleton.h"
#include "DObject.h"
#include "Entity.h"
#include "EntityDescriptor.h"
//#include "Engine.h"

//Particles
#include "ParticleSorter.h"
#include "Particle.h"
#include "ParticleSystem.h"
#include "ParticleGroup.h"
#include "ParticleGuru.h"


/*************************************************************************
	SCRIPTING
*************************************************************************/
#include "DScriptGuru.h"
#include "DScriptDisciple.h"
#include "DScriptLibraries.h"

/*************************************************************************
	INPUT
*************************************************************************/
#include "InputGuru.h"


//System -> Components
#include "ComponentDescriptor.h"
#include "ComponentDisciple.h"
#include "GuruComponentFactory.h"

//Utilities
#include "Logger.h"


/*************************************************************************
 APPLICATION LAYER INCLUDES
*************************************************************************/


#endif
