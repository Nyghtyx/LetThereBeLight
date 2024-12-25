#pragma once

#include "Scene.h"
#include "EntityManager.hpp"
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>

class Scene_Light : public Scene
{
	struct Intersect
	{
		bool result;
		Vec2f pos;
		float angle;
	};

	bool m_drawRays = false;
	sf::Shader m_lightShader;

	void init();
	void update();
	void onEnd();
	void spawnLightSource();
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

	Scene_Light(GameEngine& gameEngine);
};