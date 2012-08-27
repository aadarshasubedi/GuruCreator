/*
Author:		Dan Strycharske
Date:
File:
Brief:
*/

#ifndef INPUTGURU_H_INCLUDED
#define INPUTGURU_H_INCLUDED


#ifdef USE_LUA
	#include "LunarHelper.h"
#endif //if using Lua

#ifdef PURE_WINDOWS
	#define WIN32_LEAN_AND_MEAN
	#include <SDL.h>
	#include <windows.h>
	#include <Xinput.h>
#else
	#include "SDL.h"
#endif

#include "DSingleton.h"
#include <array>

const unsigned int LEFT_CLICK = 0;
const unsigned int RIGHT_CLICK = 1;
const unsigned int MIDDLE_CLICK = 2;


//A new class to represent the xbox 360 controller.  This only works for windows machines.
#ifdef PURE_WINDOWS
class XBoxController 
{
public:
	XBoxController(unsigned int playerNum);
	~XBoxController();

	//Some basic functionality to get states and other needed data, or to perform an action on the controller like vibration.
	XINPUT_STATE	GetState();
	bool			IsConnected();
	void			Vibrate( unsigned int left = 0, unsigned int right = 0 );

	bool CheckButtonPressed(int buttonID);

	//hacked to check for 4 buttons at once
	bool ButtonPressed( );
	bool ButtonHeld( );
	bool ButtonReleased( );
	bool ButtonUp( );

private:
	unsigned int mControlNum;
	XINPUT_STATE mControlState;
	XINPUT_STATE mPrevControlState;
};
#endif


class DInputGuru : public DSingleton<DInputGuru>
{
	private:
		//These will be the states of each of the inputs, ie. keydown, keyup, etc.
		Uint8 	*keyboardCurrState;
		Uint8 	*keyboardPrevState;
		int		numKeys;
		
		//I have these set up so that I can keep track of when the mouse clicks and things
		// happen.
		std::array<unsigned int, 9> 	mouseCurrentState;
		std::array<unsigned int, 9> 	mousePreviousState;
		
		int 	numMouseKeys;
		bool	mouseInMotion;

	public:
		DInputGuru();
		~DInputGuru();

		//Methods to access the various key, mouse, and joy stick states.		
		void PollAllInputDevices();
		void PollKeyboard();
		void PollMouse();

		bool IsKeyPressed( SDLKey keyVal );
		bool IsKeyReleased( SDLKey keyVal );
		bool IsKeyHeld( SDLKey keyVal );
		bool IsKeyUp( SDLKey keyVal );

		bool MouseClick( int mouseVal );
		bool MouseRelease( int mouseVal );
		bool MouseHeld( int mouseVal );

		bool MouseMoving();

		bool MouseScrollingUp();
		bool MouseScrollingDown();

		Uint8 GetKeyState( int keyVal );
		Uint8 GetMouseState( int keyVal );

		int GetMouseDX();
		int GetMouseDY();

		//pixel coordinates.
		int		curx, cury;
		int		px, py;
		//The difference each frame.  Nice to have as well.
		int     dx, dy;
		
#ifdef PURE_WINDOWS
		XBoxController	mXController;
#endif

#ifdef USE_LUA
		LUA_DECLARE_CLASS( DInputGuru )
		LUA_DECLARE_METHOD( LIsKeyPressed )
		LUA_DECLARE_METHOD( LIsKeyReleased )
		LUA_DECLARE_METHOD( LIsKeyHeld )
		LUA_DECLARE_METHOD( LIsKeyUp )
#endif //if using Lua
};


#endif // INPUTGURU_H_INCLUDED
