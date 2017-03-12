#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Animation.h"
#include "InputManager.h"
#include <map>
#include "PugiXml\src\pugixml.hpp"

class Sprite;

enum ACTION_STATE
{
	IDLE = 0,
	WALKING,
};

enum DIRECTION
{
	D_UP,
	D_DOWN,
	D_RIGHT,
	D_LEFT
};

class Player : public InputListener
{
public:

	Player();
	bool Awake(pugi::xml_node& config);
	bool Start();
	bool PreUpdate();
	bool UpdateTicks();
	bool Update(float dt);
	bool PostUpdate();
	bool CleanUp();
	iPoint GetWorldPosition();

	//Check different player status
	iPoint worldPosition;

private:

	bool loadAnimations();

	void Change_direction();

	

private:

	//PlayerAttributes* attributes = NULL;
		// position in pixels of player // This is the point of the pivot in the world //The feet of the player
	iPoint mapPosition;		// position of the tile where player is


	Sprite* sprite = NULL;
	SDL_Texture* playerTex;

	std::map<std::pair<ACTION_STATE, DIRECTION>, Animation>	playerAnim;
	Animation* current_animation;
	ACTION_STATE player_state = IDLE;
	DIRECTION current_direction = D_DOWN;

	bool moving = false;
	void OnInputCallback(INPUTEVENT, EVENTSTATE);
};

#endif // !_PLAYER_H_
