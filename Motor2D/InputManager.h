#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "j1Module.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>
#include <string>
#include <map>

enum INPUTEVENT
{
	ATTACK = 0,
	MUP,
	MDOWN,
	MLEFT,
	MRIGHT,
};

enum EVENTSTATE
{
	E_DOWN,
	E_UP,
	E_REPEAT
};

class InputManager: public j1Module
{
public:

	InputManager();

	// Destructor
	virtual ~InputManager();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Called before all Updates
	bool PreUpdate();

	bool update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//When detected input
	void InputEvent(int button, EVENTSTATE state);

private:

	
	//Mapping is fun
	//All the actions possible int->button, Action->attack, moveup...
	std::multimap<int, INPUTEVENT> actions;

	//All the actions in this frame
	std::multimap<INPUTEVENT, EVENTSTATE> current_action;
};

#endif // __INPUT_MANAGER_H__