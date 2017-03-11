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

bool InputManager::Update(float dt)
{
	CallListeners();

	return true;
}

// Called after all Updates
bool InputManager::PostUpdate()
{
	if(!current_action.empty())
		current_action.clear();

	return true;
}

// Called before quitting
bool InputManager::CleanUp()
{
	bool ret = true;

	

	return ret;
}

void InputManager::InputDetected(int button, EVENTSTATE state)
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

bool InputManager::ChangeEventButton(INPUTEVENT event_to_change)
{
	bool ret = false;
	static SDL_Event event;

	//Stop the game until new input event
	SDL_WaitEvent(&event);

	if (event.type == SDL_CONTROLLERBUTTONDOWN)
	{
		//Look if the new button is actually asigned
		multimap<int, INPUTEVENT>::iterator tmp = actions.find(event.cbutton.button);

		if (tmp != actions.end())
		{
			LOG("This button is actually in another action");
			return ret;
		}
		std::pair<int, INPUTEVENT> event_changed;
		event_changed = *tmp;
		actions.erase(tmp);

		actions.insert(event_changed);
		ret = true;
	}

	return ret;
}

EVENTSTATE InputManager::EventPressed(INPUTEVENT action) const
{
	multimap<INPUTEVENT, EVENTSTATE>::const_iterator tmp = current_action.find(action);

	if (tmp != current_action.end())
		return tmp->second;

	return E_NOTHING;
}

void InputManager::AddListener(InputListener* new_listener)
{
	//To improve this: Search if the listener is actually in the list

	if (new_listener)
	{
		new_listener->input_active = true;
		listeners.push_back(new_listener);
	}
		
}

void InputManager::CallListeners()
{
	if (!current_action.empty())
	{
		for (list<InputListener*>::iterator it = listeners.begin(); it != listeners.end(); it++)
		{	
			if ((*it)->input_active)
			{
				std::multimap<INPUTEVENT, EVENTSTATE>::iterator frame_actions = current_action.begin();
				while (frame_actions != current_action.end())
				{
					(*it)->OnInputCallback(frame_actions->first, frame_actions->second);
					frame_actions++;
				}
			}
		}
	}
}



