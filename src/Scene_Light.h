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

	bool m_drawRays = true;

	void init();
	void update();
	void onEnd();
	void spawnLightSource();
	void spawnPolygons();
	void drawLinesToVertices(std::shared_ptr<Entity> polygon);
	void drawPoint(const Vec2f& p, const sf::Color& color = { 255, 255, 255 });
	void drawLineToIntersections(std::shared_ptr<Entity> lightSource, std::shared_ptr<Entity> polygon);
	
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