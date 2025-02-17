#include "j1GameLayer.h"
#include "j1App.h"
#include "Player.h"
#include "j1EntityManager.h"
#include "j1Gui.h"

//#include "Attributes.h"
//#include "Hud.h"

j1GameLayer::j1GameLayer() : j1Module()
{
	//Init modules
	player = new Player();
	em = new j1EntityManager();
	//hud = new Hud();

	//player->attributes->setHud(hud);
	//player->attributes->setReferences(&player->worldPosition.x, &player->worldPosition.y);
	//hud->playerAtt = player->attributes;
}

//Destructor
j1GameLayer::~j1GameLayer()
{
	RELEASE(hud);
	RELEASE(em);
	RELEASE(player);
}

// Called before render is available
bool j1GameLayer::Awake(pugi::xml_node& conf)
{
	player->Awake(conf);
	em->Awake(conf);
	//hud->Awake(conf);
	return true;
}

// Called before the first frame
bool j1GameLayer::Start()
{
	active = true;

	player->Start();
	em->Start();
	//hud->Start();

	return true;
}

//preUpdate
bool j1GameLayer::PreUpdate()
{
	player->PreUpdate();
	em->PreUpdate();
	//hud->PreUpdate();

	return true;
}

//update
bool j1GameLayer::Update(float dt)
{
	bool ret = true;

	player->Update(dt);
	em->Update(dt);
	//ret = hud->Update(dt);

	//player->Draw();

	return ret;
}

//postUpdate
bool j1GameLayer::PostUpdate()
{
	player->PostUpdate();
	em->PostUpdate();
	//hud->PostUpdate();

	return true;
}

// Called before quitting
bool j1GameLayer::CleanUp()
{
	active = false;

	//hud->CleanUp();
	em->CleanUp();
	player->CleanUp();

	return true;
}