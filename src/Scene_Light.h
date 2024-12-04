#pragma once

#include "Scene.h"
#include "EntityManager.hpp"
#include <memory>

class Scene_Light : public Scene
{
	void init();
	void update();
	void onEnd();
	void spawnLightSource();
	
	void sDoAction(const Action& action);
	void sCollision();
	void sMovement();
	void sLighting();
	void sRender();

	std::shared_ptr<Entity> light();

public:

	Scene_Light(GameEngine& gameEngine);
};