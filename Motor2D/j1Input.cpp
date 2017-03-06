
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
#include "SDL/include/SDL.h"

#define MAX_KEYS 300

j1Input::j1Input() : j1Module()
{
	name = ("input");

	keyboard = new j1KeyState[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(j1KeyState) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_MOUSE_BUTTONS);
	memset(controller_buttons, KEY_IDLE, sizeof(j1KeyState) * NUM_CONTROLLER_BUTTONS);
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
	SDL_Init(SDL_INIT_JOYSTICK | SDL_INIT_GAMECONTROLLER);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
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
			/*joystick = SDL_JoystickOpen(i);

			if (joystick == nullptr)
				LOG("Gamepad not opened %s", SDL_GetError());*/
			gamepad = SDL_GameControllerOpen(i);
			if(gamepad == nullptr)
				LOG("Gamepad not opened %s", SDL_GetError());

		}

	}

	int i =  SDL_GameControllerAddMapping("00000000000000000000000000000000,X360 Controller,a:b0,b:b11,back:b5,dpdown:b1,dpleft:b2,dpright:b3,dpup:b10,guide:b14,leftshoulder:b8,leftstick:b6,lefttrigger:a4,leftx:a0,lefty:a1,rightshoulder:b9,rightstick:b7,righttrigger:a5,rightx:a2,righty:a3,start:b4,x:b12,y:b13,");

	
	SDL_StopTextInput();
	return true;
}

// Called each loop iteration
bool j1Input::PreUpdate()
{
	static SDL_Event event;
	const Uint8* keys = SDL_GetKeyboardState(NULL);

	for(int i = 0; i < MAX_KEYS; ++i)
	{
		//Translate key number to scancode enum
		SDL_Scancode code = (SDL_Scancode)i;

		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
			{
				keyboard[i] = KEY_DOWN;
				down_queue.push(SDL_GetScancodeName(code));
			}
			else
			{
				keyboard[i] = KEY_REPEAT;
				repeat_queue.push(SDL_GetScancodeName(code));
			}
		}
		else
		{
			if (keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
			{
				keyboard[i] = KEY_UP;
				up_queue.push(SDL_GetScancodeName(code));
			}

			else
			{
				keyboard[i] = KEY_IDLE;
			}
		}
	}

	for(int i = 0; i < NUM_MOUSE_BUTTONS; ++i)
	{
		if(mouse_buttons[i] == KEY_DOWN)
			mouse_buttons[i] = KEY_REPEAT;

		if(mouse_buttons[i] == KEY_UP)
			mouse_buttons[i] = KEY_IDLE;
	}

	for (int i = 0; i < NUM_CONTROLLER_BUTTONS; ++i)
	{
		if (controller_buttons[i] == KEY_DOWN)
			controller_buttons[i] = KEY_REPEAT;

		if (controller_buttons[i] == KEY_UP)
			controller_buttons[i] = KEY_IDLE;
	}

	
	mouse_motion_x = 0;
	mouse_motion_y = 0;
	mouse_wheel = 0;
	joystick_x = joystick_y = 0;
	joy = false;

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

			case SDL_MOUSEBUTTONDOWN:
				mouse_buttons[event.button.button - 1] = KEY_DOWN;
			break;

			case SDL_MOUSEBUTTONUP:
				mouse_buttons[event.button.button - 1] = KEY_UP;
			break;

			case SDL_MOUSEMOTION:
			{
				int scale = App->win->GetScale();
				mouse_motion_x = event.motion.xrel / scale;
				mouse_motion_y = event.motion.yrel / scale;
				mouse_x = event.motion.x / scale;
				mouse_y = event.motion.y / scale;

				break;
			}
			case SDL_MOUSEWHEEL:
				mouse_wheel = event.wheel.y * 10;
				break;

			case SDL_KEYDOWN:

				if(event.key.keysym.scancode == SDL_SCANCODE_GRAVE)
					App->console->Active_console();

				break;
		
			case SDL_TEXTINPUT:
			
				//LOG("estoy escribiendo");
				if (App->gui->element_selected && App->gui->element_selected->element_type == UI_TYPE::TEXT_BOX)
				{
						UI_Text_Box* temp = (UI_Text_Box*)App->gui->element_selected;
						temp->Insert_Char(temp->cursor_virtual_pos, event.text.text);
					
						int width;
						App->font->CalcSize(event.text.text, width, temp->height);
						temp->cursor_virtual_pos++;
						temp->cursor_pos += width;
				}
				
				break;
	
			case SDL_CONTROLLERAXISMOTION:

				//if(event.caxis.axis == SDL_CONTROLLER_AXIS_LEFTX)
					//LOG(" %i", event.caxis.value);
				break;

			case SDL_CONTROLLERBUTTONDOWN:

				LOG("BOTON: %i", event.cbutton.button);
				//controller_buttons[event.cbutton.button] = KEY_DOWN;
				break;

			case SDL_CONTROLLERBUTTONUP:
				controller_buttons[event.cbutton.button] = KEY_DOWN;
				break;

			case SDL_CONTROLLERDEVICEADDED:
				
				if (SDL_IsGameController(event.cdevice.which))
					gamepad = SDL_GameControllerOpen(event.cdevice.which);

				break;
		}
	}

	return true;
}

// Called before quitting
bool j1Input::CleanUp()
{
	LOG("Quitting SDL event subsystem");

	SDL_GameControllerClose(gamepad);

	SDL_QuitSubSystem(SDL_INIT_EVENTS);
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
