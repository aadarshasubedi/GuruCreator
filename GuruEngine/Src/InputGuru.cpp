/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/


#include "InputGuru.h"
#include "Engine.h"


DInputGuru::DInputGuru() 
#ifdef PURE_WINDOWS 
	: mXController(1)
#endif
{
	SDL_GetKeyState( &numKeys );
	keyboardCurrState = new Uint8[numKeys];
	keyboardPrevState = new Uint8[numKeys];		

	curx = cury = 0;
	px = py = 0;
	mouseInMotion = false;

	mousePreviousState[0] = 0;
	mousePreviousState[1] = 0;
	mousePreviousState[2] = 0;
	mousePreviousState[3] = 0;
	mousePreviousState[4] = 0;
	mousePreviousState[5] = 0;
	mousePreviousState[6] = 0;
	mousePreviousState[7] = 0;

	mouseCurrentState[0] = 0;
	mouseCurrentState[1] = 0;
	mouseCurrentState[2] = 0;
	mouseCurrentState[3] = 0;
	mouseCurrentState[4] = 0;
	mouseCurrentState[5] = 0;
	mouseCurrentState[6] = 0;
	mouseCurrentState[7] = 0;
}

DInputGuru::~DInputGuru()
{
    delete [] keyboardCurrState;
    delete [] keyboardPrevState;
}


//Methods to access the various key, mouse, and joy stick states.
void DInputGuru::PollAllInputDevices()
{
	PollMouse();
	PollKeyboard();
#ifdef PURE_WINDOWS
	mXController.GetState();

	mXController.ButtonPressed();
	mXController.ButtonHeld();
	mXController.ButtonReleased();
	mXController.ButtonUp();
#endif
}

void DInputGuru::PollKeyboard()
{
	//Grab the keyboard state and copy it over to the current state buffer.
	static Uint8* keystate = SDL_GetKeyState( &numKeys );

	//Take current state and make it previous state.
	memcpy( keyboardPrevState, keyboardCurrState, numKeys );

	//Then memcpy to update the current internal state with the SDL provided keyboard state.
	memcpy( keyboardCurrState, keystate, numKeys );
}

void DInputGuru::PollMouse()
{
	px = curx;
	py = cury;

	int button = SDL_GetMouseState(&curx, &cury);

	dx = curx - px;
	dy = cury - py;
	
	if( dx != 0 || dy != 0 )
	{
		mousePreviousState[0] = mouseCurrentState[0];
		mouseCurrentState[0] = 1;
	}
	else
	{
		mousePreviousState[0] = mouseCurrentState[0];
		mouseCurrentState[0] = 0;
	}

	if( button & SDL_BUTTON_LMASK )
	{
		//Set previous left button state, update current.
		mousePreviousState[SDL_BUTTON_LEFT] = mouseCurrentState[SDL_BUTTON_LEFT];
		mouseCurrentState[SDL_BUTTON_LEFT] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_LEFT] = mouseCurrentState[SDL_BUTTON_LEFT];
		mouseCurrentState[SDL_BUTTON_LEFT] = 0;
	}

	if( button & SDL_BUTTON_MMASK )
	{
		//Set previous left button state, update current.
		mousePreviousState[SDL_BUTTON_MIDDLE] = mouseCurrentState[SDL_BUTTON_MIDDLE];
		mouseCurrentState[SDL_BUTTON_MIDDLE] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_MIDDLE] = mouseCurrentState[SDL_BUTTON_MIDDLE];
		mouseCurrentState[SDL_BUTTON_MIDDLE] = 0;
	}

	if( button & SDL_BUTTON_RMASK )
	{
		//Set previous left button state, update current.
		mousePreviousState[SDL_BUTTON_RIGHT] = mouseCurrentState[SDL_BUTTON_RIGHT];
		mouseCurrentState[SDL_BUTTON_RIGHT] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_RIGHT] = mouseCurrentState[SDL_BUTTON_RIGHT];
		mouseCurrentState[SDL_BUTTON_RIGHT] = 0;
	}

	if( button & SDL_BUTTON(SDL_BUTTON_WHEELUP) )
	{
		mousePreviousState[SDL_BUTTON_WHEELUP] = mouseCurrentState[SDL_BUTTON_WHEELUP];
		mouseCurrentState[SDL_BUTTON_WHEELUP] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_WHEELUP] = mouseCurrentState[SDL_BUTTON_WHEELUP];
		mouseCurrentState[SDL_BUTTON_WHEELUP] = 0;
	}

	if( button & SDL_BUTTON(SDL_BUTTON_WHEELDOWN) )
	{
		mousePreviousState[SDL_BUTTON_WHEELDOWN] = mouseCurrentState[SDL_BUTTON_WHEELDOWN];
		mouseCurrentState[SDL_BUTTON_WHEELDOWN] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_WHEELDOWN] = mouseCurrentState[SDL_BUTTON_WHEELDOWN];
		mouseCurrentState[SDL_BUTTON_WHEELDOWN] = 0;
	}

	if( button & SDL_BUTTON_X1MASK )
	{
		mousePreviousState[SDL_BUTTON_X1] = mouseCurrentState[SDL_BUTTON_X1];
		mouseCurrentState[SDL_BUTTON_X1] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_X1] = mouseCurrentState[SDL_BUTTON_X1];
		mouseCurrentState[SDL_BUTTON_X1] = 0;
	}

	if( button & SDL_BUTTON_X2MASK )
	{
		mousePreviousState[SDL_BUTTON_X2] = mouseCurrentState[SDL_BUTTON_X2];
		mouseCurrentState[SDL_BUTTON_X2] = 1;
	}
	else
	{
		mousePreviousState[SDL_BUTTON_X2] = mouseCurrentState[SDL_BUTTON_X2];
		mouseCurrentState[SDL_BUTTON_X2] = 0;
	}
}


bool DInputGuru::IsKeyPressed( SDLKey keyVal )
{
	if( keyboardCurrState[keyVal] && !keyboardPrevState[keyVal] )
	{
		return true;
	}

	return false;
}

bool DInputGuru::IsKeyReleased( SDLKey keyVal )
{
	if( !keyboardCurrState[keyVal] && keyboardPrevState[keyVal] )
	{
		return true;
	}

	return false;
}

bool DInputGuru::IsKeyHeld( SDLKey keyVal )
{
	if( keyboardCurrState[keyVal] && keyboardPrevState[keyVal] )
	{
		return true;
	}

	return false;
}

bool DInputGuru::IsKeyUp( SDLKey keyVal )
{
	if( !keyboardCurrState[keyVal] && !keyboardPrevState[keyVal] )
	{
		return true;
	}

	return false;
}


bool DInputGuru::MouseClick( int mouseVal )
{
	if( mouseVal == SDL_BUTTON_LEFT || mouseVal == SDL_BUTTON_RIGHT ||
		mouseVal == SDL_BUTTON_X1   || mouseVal == SDL_BUTTON_X2 )
	{
		if( mouseCurrentState[mouseVal] && !mousePreviousState[mouseVal] )
		{
			return true;
		}
		return false;
	}
	return false;
}

bool DInputGuru::MouseRelease( int mouseVal )
{
	if( mouseVal == SDL_BUTTON_LEFT || mouseVal == SDL_BUTTON_RIGHT ||
		mouseVal == SDL_BUTTON_X1   || mouseVal == SDL_BUTTON_X2 )
	{
		if( !mouseCurrentState[mouseVal] && mousePreviousState[mouseVal] )
		{
			return true;
		}

		return false;
	}
	return false;
}

bool DInputGuru::MouseHeld( int mouseVal )
{
	if( mouseVal == SDL_BUTTON_LEFT || mouseVal == SDL_BUTTON_RIGHT ||
		mouseVal == SDL_BUTTON_X1   || mouseVal == SDL_BUTTON_X2 )
	{
		if( mouseCurrentState[mouseVal] && mousePreviousState[mouseVal] )
		{
			return true;
		}

		return false;
	}
	return false;
}


bool DInputGuru::MouseMoving()
{
	return (mouseCurrentState[0]) ? true : false;
}


bool DInputGuru::MouseScrollingUp()
{
	if( mouseCurrentState[SDL_BUTTON_WHEELUP] )
	{
		return true;
	}
	return false;
}

bool DInputGuru::MouseScrollingDown()
{
	if( mouseCurrentState[SDL_BUTTON_WHEELDOWN] )
	{
		return true;
	}
	return false;
}


Uint8 DInputGuru::GetKeyState( int  )
{
	return 0;
}

Uint8 DInputGuru::GetMouseState( int  )
{
	return 0;
}


int DInputGuru::GetMouseDX()
{
    return dx;
}

int DInputGuru::GetMouseDY()
{
    return dy;
}


/////////////////////////////////////////////////////////////////////////////////
////////////////////////XBox Controller//////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////


#ifdef PURE_WINDOWS

//playernum is coming in as 1 - 4, but the controllers are indexed as 0 - 3, thus the minus 1.
XBoxController::XBoxController( unsigned int playerNum )
{
	mControlNum = playerNum - 1;
	ZeroMemory( &mControlState, sizeof( XINPUT_STATE) );
	ZeroMemory( &mPrevControlState, sizeof( XINPUT_STATE) );
}

XBoxController::~XBoxController()
{
}

//Some basic functionality to get states and other needed data, or to perform an action on the controller like vibration.
XINPUT_STATE	XBoxController::GetState()
{
	mPrevControlState = mControlState;
	ZeroMemory( &mControlState, sizeof( XINPUT_STATE) );

	XInputGetState( mControlNum, &mControlState );

	return mControlState;
}

bool XBoxController::IsConnected()
{
	DWORD result = XInputGetState( mControlNum, &mControlState );
	if( result == ERROR_SUCCESS )
	{
		return true;
	}
	else
	{
		return false;
	}

}

void XBoxController::Vibrate( unsigned int left, unsigned int right )
{
	//Create a vibration state.
	XINPUT_VIBRATION vibration;

	//zero out the initial values of the state.
	ZeroMemory( &vibration, sizeof( XINPUT_VIBRATION ) );

	vibration.wLeftMotorSpeed = (WORD)left;
	vibration.wRightMotorSpeed = (WORD)right;

	XInputSetState( mControlNum, &vibration );
}

bool XBoxController::CheckButtonPressed(int buttonID)
{
	bool held = mControlState.Gamepad.wButtons & buttonID ? true : false;
	bool prevHeld = mPrevControlState.Gamepad.wButtons & buttonID ? true : false;
	if (held && !prevHeld)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool XBoxController::ButtonPressed( )
{
	static int ButtonList[4] = {
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y
	};

	for( int i=0; i<4; ++i )
	{
		bool held = mControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		bool prevHeld = mPrevControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		if (held && !prevHeld)
		{
			return true;
		}
	}
	return false;
}

bool XBoxController::ButtonHeld( )
{
	static int ButtonList[4] = {
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y
	};

	for( int i=0; i<4; ++i )
	{
		bool held = mControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		bool prevHeld = mPrevControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		if (held && prevHeld)
		{
			return true;
		}
	}
	return false;
}

bool XBoxController::ButtonReleased( )
{
	static int ButtonList[4] = {
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y
	};

	for( int i=0; i<4; ++i )
	{
		bool held = mControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		bool prevHeld = mPrevControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		if (!held && prevHeld)
		{
			return true;
		}
	}
	return false;
}

bool XBoxController::ButtonUp( )
{
	static int ButtonList[4] = {
		XINPUT_GAMEPAD_A,
		XINPUT_GAMEPAD_B,
		XINPUT_GAMEPAD_X,
		XINPUT_GAMEPAD_Y
	};

	for( int i=0; i<4; ++i )
	{
		bool held = mControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		bool prevHeld = mPrevControlState.Gamepad.wButtons & ButtonList[i] ? true : false;
		if (!held && !prevHeld)
		{
			return true;
		}
	}
	return false;
}

#endif // windows only for xbox controller

#ifdef USE_LUA
LUA_START_DEFINE_CLASS( DInputGuru )
	LUA_NAME_METHOD( DInputGuru, "IsKeyPressed", LIsKeyPressed )
	LUA_NAME_METHOD( DInputGuru, "IsKeyReleased", LIsKeyReleased )
	LUA_NAME_METHOD( DInputGuru, "IsKeyHeld", LIsKeyHeld )
	LUA_NAME_METHOD( DInputGuru, "IsKeyUp", LIsKeyUp )
LUA_END_DEFINE_CLASS

LUA_DEFINE_METHOD( DInputGuru, LIsKeyPressed )
{
	//"ls" is an automatic parameter
	bool result = IsKeyPressed( SDLK_SPACE );
	lua_pushboolean(ls, result );
	return 1;
}
LUA_DEFINE_METHOD( DInputGuru, LIsKeyReleased )
{
	//"ls" is an automatic parameter
	bool result = IsKeyReleased( SDLK_SPACE );
	lua_pushboolean(ls, result );
	return 1;
}
LUA_DEFINE_METHOD( DInputGuru, LIsKeyHeld )
{
	//"ls" is an automatic parameter
	bool result = IsKeyHeld( SDLK_SPACE );
	lua_pushboolean(ls, result );
	return 1;
}
LUA_DEFINE_METHOD( DInputGuru, LIsKeyUp )
{
	//"ls" is an automatic parameter
	bool result = IsKeyUp( SDLK_SPACE );
	lua_pushboolean(ls, result );
	return 1;
}
#endif //if using Lua