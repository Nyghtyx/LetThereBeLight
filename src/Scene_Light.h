#pragma once

#include "Scene.h"
#include "EntityManager.hpp"
#include <memory>

class Scene_Light : public Scene
{
	struct Intersect
	{
		bool result;
		Vec2f pos;
	};

	void init();
	void update();
	void onEnd();
	void spawnLightSource();
	void spawnPolygons();
	
	void sDoAction(const Action& action);
	void sCollision();
	void sMovement();
	void sLighting();
	void sRender();

	std::shared_ptr<Entity> light();
	Intersect lineIntersect(const Vec2f& a, const Vec2f& b, const Vec2f& c, const Vec2f& d);

public:

	Scene_Light(GameEngine& gameEngine);
};