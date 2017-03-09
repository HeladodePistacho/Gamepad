#include "InputManager.h"
#include "j1App.h"
#include "p2Defs.h"
#include "p2Log.h"
#include "j1FileSystem.h"
#include "j1Input.h"
#include "j1Fonts.h"
#include "j1Audio.h"
#include "SDL\include\SDL.h"

InputManager::InputManager() : j1Module()
{
	name = "inputManager";
}

// Destructor
InputManager::~InputManager()
{}

// Called when before render is available
bool InputManager::Awake(pugi::xml_node& conf)
{
	bool ret = true;

	//Load All actions
	for (pugi::xml_node tmp = conf.child("action"); tmp != nullptr; tmp = tmp.next_sibling())
	{
		std::pair<int, INPUTEVENT> new_action;
		new_action.first = tmp.attribute("button").as_int();
		new_action.second = (INPUTEVENT)tmp.attribute("event").as_int();

		actions.insert(new_action);
	}

 	
	return ret;
}

// Called before all Updates
bool InputManager::PreUpdate()
{
	bool ret = true;

	

	
	return ret;
}

bool InputManager::update(float dt)
{
	/*bool ret = true;

	// Check for shortcut change
	list<ShortCut*>::iterator it = shortcuts_list.begin();
	while (it != shortcuts_list.end())
	{
		if ((*it)->ready_to_change)
		{
			static SDL_Event event;

			// COMPLETELY STOPS THE GAME...
			SDL_WaitEvent(&event);

			if (event.type == SDL_KEYDOWN)
			{
				std::string code = SDL_GetScancodeName(event.key.keysym.scancode);

				bool keyAlreadyAssigned = false;

				list<ShortCut*>::iterator it2 = shortcuts_list.begin();
				for (;it2 != shortcuts_list.end() && !keyAlreadyAssigned; it2++)
				{
					keyAlreadyAssigned = (strcmp((*it2)->command.c_str(), code.c_str()) == 0);
				}

				if (!keyAlreadyAssigned)
				{
					(*it)->command = code;
					ChangeShortcutCommand((*it));

					/*list<ShortCut*>::iterator it = shortcuts_list.begin();
					while (it != shortcuts_list.end())
					{
						(*it)->active = false;
						++it;
					}//
				}
				else
				{
					LOG("Key already assigned");
				}
			}

			(*it)->ready_to_change = false;

			return ret;
		}

		++it;
	}

	return ret;*/

	return true;
}

// Called after all Updates
bool InputManager::PostUpdate()
{


	return true;
}

// Called before quitting
bool InputManager::CleanUp()
{
	bool ret = true;

	

	return ret;
}

void InputManager::InputEvent(int button, EVENTSTATE state)
{

	multimap<int, INPUTEVENT>::iterator tmp = actions.find(button);
	//If more than one action per button we must iterate until the end
	if (tmp != actions.end())
	{
		std::pair<INPUTEVENT, EVENTSTATE> new_current_action;
		new_current_action.first = (*tmp).second;
		new_current_action.second = state;
		current_action.insert(new_current_action);
	}
}



