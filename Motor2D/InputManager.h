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
	JUMP = 0,
	MUP,
	MDOWN,
	MLEFT,
	MRIGHT,
};

enum EVENTSTATE
{
	E_NOTHING,
	E_DOWN,
	E_UP,
	E_REPEAT
};

class InputListener
{
public:
	InputListener() : input_active(false) {}
	bool input_active;
	virtual void OnInputCallback(INPUTEVENT, EVENTSTATE) {};
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

	bool Update(float dt);

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	//When detected input
	void InputDetected(int, EVENTSTATE);

	//To Change the action button
	bool ChangeEventButton(INPUTEVENT);

	//For Polling
	EVENTSTATE EventPressed(INPUTEVENT) const;

	//For Callback system
	void AddListener(InputListener*);
	void CallListeners();

private:

	
	//Mapping is fun
	//All the actions possible int->button, INPUTEVENT->attack, moveup...
	std::multimap<int, INPUTEVENT> actions;

	//All the actions in this frame
	std::multimap<INPUTEVENT, EVENTSTATE> current_action;

	std::list<InputListener*> listeners;
};

#endif // __INPUT_MANAGER_H__