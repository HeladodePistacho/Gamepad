#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include "j1Module.h"
#include "p2Log.h"
#include "j1App.h"
#include "j1Input.h"
#include "PugiXml\src\pugixml.hpp"
#include <list>
#include <string>


enum INPUT_TYPE
{
	DOWN,
	UP,
	REPEAT
};

struct Action
{
	Action(INPUT_TYPE new_type, string new_name, SDL_GameControllerButton new_button) : type(new_type), name(new_name), button(new_button) {}

	bool			active = false;
	bool			ready_to_change = false;

	INPUT_TYPE		type;
	std::string		name;
	int				button;

};

enum ACTIONID
{
	A = 0,
	B,
	DUP = 11,
	DDOWN,
	DLEFT,
	DRIGHT,
};



class InputManager: public j1Module
{
public:

	InputManager();

	// Destructor
	virtual ~InputManager();

	// Called when before render is available
	bool awake(pugi::xml_node&);

	// Called before all Updates
	bool preUpdate();

	bool update(float dt);

	// Called after all Updates
	bool postUpdate();

	// Called before quitting
	bool cleanUp();

	//Action list
	std::list<Action*>		actions_list;

	//Check for shortcut state
	//bool CheckShortcut(ShortCutID id = A);
	bool CheckAction(ACTIONID id);

private:

	//Refresh commands once have been changed
	//void ChangeShortcutCommand(ShortCut* shortcut);

	//Shortcuts xml file path
	std::string		inputs_file_path;
};

#endif // __INPUT_MANAGER_H__