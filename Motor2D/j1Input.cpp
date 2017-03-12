
#include "p2Defs.h"
#include "p2Log.h"
#include "j1Fonts.h"
#include "j1App.h"
#include "j1Input.h"
#include "j1Window.h"
#include "j1Console.h"
#include "UI_Text_Box.h"
#include "j1Gui.h"
#include "j1GameLayer.h"
#include "Player.h"
#include "InputManager.h"
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name = ("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
}

// Destructor
j1Input::~j1Input()
{
	

	delete[] keyboard;
}

// Called before render is available
bool j1Input::Awake(pugi::xml_node& config)
{
	LOG("Init SDL input event system");
	bool ret = true;
	//SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS | SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called before the first frame
bool j1Input::Start()
{
	int num_joysticks = SDL_NumJoysticks();
	for (int i = 0; i < SDL_NumJoysticks(); ++i)
	{
		if (SDL_IsGameController(i))
		{
			gamepad = SDL_GameControllerOpen(i);
			if(gamepad == nullptr)
				LOG("Gamepad not opened %s", SDL_GetError());

		}

	}

SDL_GameControllerAddMapping("00000000000000000000000000000000,X360 Controller,a:b10,b:b11,back:b5,dpdown:b1,dpleft:b2,dpright:b3,dpup:b0,guide:b14,leftshoulder:b8,leftstick:b6,lefttrigger:a4,leftx:a0,lefty:a1,rightshoulder:b9,rightstick:b7,righttrigger:a5,rightx:a2,righty:a3,start:b4,x:b12,y:b13,");

	
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	
	static SDL_Event event;	

	for (int i = 0; i < NUM_CONTROLLER_BUTTONS; ++i)
	{
		if (controller_buttons[i] == KEY_DOWN || controller_buttons[i] == KEY_REPEAT)
		{
			controller_buttons[i] = KEY_REPEAT;
			App->inputM->InputDetected(i, EVENTSTATE::E_REPEAT);
		}
			
		if (controller_buttons[i] == KEY_UP)
			controller_buttons[i] = KEY_IDLE;
	}


	while(SDL_PollEvent(&event) != 0)
	{
		switch(event.type)
		{
			case SDL_QUIT:
				windowEvents[WE_QUIT] = true;
			break;

			case SDL_WINDOWEVENT:
				switch(event.window.event)
				{
					//case SDL_WINDOWEVENT_LEAVE:
					case SDL_WINDOWEVENT_HIDDEN:
					case SDL_WINDOWEVENT_MINIMIZED:
					case SDL_WINDOWEVENT_FOCUS_LOST:
					windowEvents[WE_HIDE] = true;
					break;

					//case SDL_WINDOWEVENT_ENTER:
					case SDL_WINDOWEVENT_SHOWN:
					case SDL_WINDOWEVENT_FOCUS_GAINED:
					case SDL_WINDOWEVENT_MAXIMIZED:
					case SDL_WINDOWEVENT_RESTORED:
					windowEvents[WE_SHOW] = true;
					break;
				}
			break;

			
			case SDL_CONTROLLERBUTTONDOWN:

				LOG("BOTON: %i", event.cbutton.button);
				controller_buttons[event.cbutton.button] = KEY_DOWN;
				App->inputM->InputDetected(event.cbutton.button, EVENTSTATE::E_DOWN);
				break;

			case SDL_CONTROLLERBUTTONUP:
				controller_buttons[event.cbutton.button] = KEY_UP;
				App->inputM->InputDetected(event.cbutton.button, EVENTSTATE::E_UP);
				break;

	
			case SDL_CONTROLLERDEVICEADDED:
				
				if (SDL_IsGameController(event.cdevice.which))
					gamepad = SDL_GameControllerOpen(event.cdevice.which);

				break;

			case SDL_CONTROLLERDEVICEREMOVED:
				if (gamepad)
					SDL_GameControllerClose(gamepad);
				break;
		}
	}

	return true;
}

bool j1Input::PostUpdate()
{

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	if (gamepad)
		SDL_GameControllerClose(gamepad);

	SDL_QuitSubSystem(SDL_INIT_EVENTS | SDL_INIT_GAMECONTROLLER);
	return true;
}

// ---------
bool j1Input::GetWindowEvent(j1EventWindow ev)
{
	return windowEvents[ev];
}

void j1Input::GetMousePosition(int& x, int& y)
{
	x = mouse_x;
	y = mouse_y;
}

void j1Input::GetMouseMotion(int& x, int& y)
{

	x = mouse_motion_x;
	y = mouse_motion_y;

}

void j1Input::GetMouseWheel(int& y)
{
	y = mouse_wheel;
}




