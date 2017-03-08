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
bool InputManager::awake(pugi::xml_node& conf)
{
	bool ret = true;

	//Loading shortcuts path xml
	inputs_file_path = conf.child("shortcuts_path").attribute("value").as_string();

	return ret;
}

// Called before all Updates
bool InputManager::preUpdate()
{
	bool ret = true;

	//DOWN
	if (!App->input->down_button_queue.empty())
	{
		for (int i = 0; i < App->input->down_button_queue.size(); i++)
		{
			for (list<Action*>::iterator item = actions_list.begin(); item != actions_list.end(); item++)
			{
				if (App->input->down_button_queue.front() + i == (*item)->button && (*item)->type == DOWN)
					(*item)->active = true;
			}
		}
	}

	
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
bool InputManager::postUpdate()
{

	for(list<Action*>::iterator it = actions_list.begin(); it != actions_list.end(); it++)
		(*it)->active = false;

	return true;
}

// Called before quitting
bool InputManager::cleanUp()
{
	bool ret = true;

	actions_list.clear();

	return ret;
}

bool InputManager::CheckAction(ACTIONID id)
{
	bool ret = false;

	for (std::list<Action*>::iterator it = actions_list.begin(); it != actions_list.end(); it++)
	{
		if (id == (*it)->button)
		{
			ret = (*it)->active;
			break;
		}
	}

	return ret;
}

/*void InputManager::ChangeShortcutCommand(ShortCut* shortcut)
{
	// manage input manager panel
	/*shortcut->command_label->SetText(shortcut->command, app->font->smallFont);

	iPoint posLabel = shortcut->command_label->getLocalPosition();
	iPoint posDecor = shortcut->decor->getLocalPosition();
	posLabel.x = posDecor.x + 63 - ((int(shortcut->command.size()) - 1) * 5);
	shortcut->command_label->SetLocalPosition(posLabel);

	shortcut->ready_to_change = false;
}*/