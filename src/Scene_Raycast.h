#pragma once

#include "Scene.h"
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Scene_Raycast : public Scene
{
	struct Intersect
	{
		bool result;
		Vec2f pos;
		float angle;
	};

	void init(Vec2f& pos);
	void update();
	void onEnd();
	void spawnLightSource(Vec2f& pos);
	void spawnPolygons();
	void drawPoint(const Vec2f& p, const sf::Color& color = { 255, 255, 255 });
	
	
	void sDoAction(const Action& action);
	void sCollision();
	void sMovement();
	void sLighting();
	void sRender();

	std::shared_ptr<Entity> light();
	Intersect lineIntersect(const Vec2f& a, const Vec2f& b, const Vec2f& c, const Vec2f& d); 
	Intersect intersectPolygons(const Vec2f& a, const Vec2f& b); 
	std::vector<Intersect> racycastToPolygons(std::shared_ptr<Entity> lightSource, std::shared_ptr<Entity> polygon);
	Vec2f rotateLineSegment(const Vec2f& a, const Vec2f& b, float angle);

public:

	Scene_Raycast(GameEngine& gameEngine, Vec2f pos);
};