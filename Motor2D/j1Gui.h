#ifndef __j1GUI_H__
#define __j1GUI_H__

#include "j1Module.h"
#include "p2List.h"
#include "p2Point.h"
#include "UI_element.h"

#define CURSOR_WIDTH 2

struct SDL_Texture;

enum GUI_INPUT
{
	MOUSE_IN,
	MOUSE_OUT,	
	RIGHT_MOUSE_DOWN,
	RIGHT_MOUSE_REPEAT,
	LEFT_MOUSE_DOWN,
	LEFT_MOUSE_REPEAT,
	ENTER

};

class j1Gui : public j1Module
{
public:

	j1Gui();

	// Destructor
	virtual ~j1Gui();

	// Called when before render is available
	bool Awake(pugi::xml_node&);

	// Call before first frame
	bool Start();

	// Called before all Updates
	bool PreUpdate();

	bool Update();

	// Called after all Updates
	bool PostUpdate();

	// Called before quitting
	bool CleanUp();

	// TODO 2: Create the factory methods
	// Gui creation functions

	UI_element* CreateScreen(UI_element* new_element);

	UI_element* Add_element(UI_TYPE, j1Module*);

	const SDL_Texture* GetAtlas() const;
	const SDL_Texture* Get_Other_Textures(uint id) const;

	UI_element* element_selected;
	UI_element* focus_element;

	int Get_tabs() const;
	void Actualize_tabs();
	void Go_Next_Tab();
	void Look_for(const UI_element* looked_element);

private:

	SDL_Texture* atlas;
	string atlas_file_name;
	list<SDL_Texture*> Other_images;
	list<UI_element*> Screen_elements;
	
	int num_of_tabs = 0;


};

#endif // __j1GUI_H__