#pragma once

#include "Scene.h"
#include "EntityManager.hpp"

class Scene_Light : public Scene
{
	void init();
	void update();
	void onEnd();
	void spawnLightSource();
	
	void sDoAction(const Action& action);
	void sMovement();
	void sLighting();
	void sRender();


public:

	Scene_Light(GameEngine& gameEngine);
};