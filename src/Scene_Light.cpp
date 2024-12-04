#include "Scene_Light.h"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <map>

Scene_Light::Scene_Light(GameEngine& gameEngine)
	: Scene(gameEngine)
{
	init();
}

void Scene_Light::init()
{
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");

	spawnLightSource();
	spawnPolygons();
}

void Scene_Light::update()
{
	m_entityManager.update();

	sMovement();
	sCollision();
	sLighting();
	sRender();

	m_currentFrame++;
}

void Scene_Light::onEnd()
{
	m_game.quit();
}

void Scene_Light::spawnLightSource()
{
	auto light = m_entityManager.addEntity("light");
	light->add<CTransform>(Vec2f(width() / 2.0f, height() / 2.0f));
	light->add<CCircleShape>(24, 32, sf::Color(247, 247, 111));
	light->add<CInput>();
}

void Scene_Light::spawnPolygons()
{
	// Hardcoded polygons for example, clearly not scalable
	// probably read from file for true implementation into game
	std::map<size_t, std::vector<Vec2f>> polyMap;
	polyMap[0] = std::vector<Vec2f>{Vec2f(100.0f, 150.0f), Vec2f(120.0f, 50.0f), Vec2f(200.0f, 80.0f), Vec2f(140.0f, 210.0f) };
	polyMap[1] = std::vector<Vec2f>{ Vec2f(100.0f, 200.0f), Vec2f(120.0f, 250.0f), Vec2f(60.0f, 300.0f) };
	polyMap[2] = std::vector<Vec2f>{ Vec2f(200.0f, 260.0f), Vec2f(220.0f, 150.0f), Vec2f(300.0f, 200.0f), Vec2f(350.0f, 320.0f) };
	polyMap[3] = std::vector<Vec2f>{ Vec2f(540.0f, 60.0f), Vec2f(560.0f, 40.0f), Vec2f(570.0f, 70.0f) };
	polyMap[4] = std::vector<Vec2f>{ Vec2f(650.0f, 190.0f), Vec2f(760.0f, 170.0f), Vec2f(740.0f, 270.0f), Vec2f(630.0f, 290.0f) };
	polyMap[5] = std::vector<Vec2f>{ Vec2f(600.0f, 95.0f), Vec2f(780.0f, 50.0f), Vec2f(680.0f, 150.0f)};

	for (auto& [i, points] : polyMap)
	{
		auto polygon = m_entityManager.addEntity("polygon");
		polygon->add<CTransform>(points[0]);
		polygon->add<CPolygon>(points.size());
		
		auto& shape = polygon->get<CPolygon>().polygon;
		for (int j = 0; j < points.size(); j++)
		{
			shape.setPoint(j, points[j]);
		}
	}
}

void Scene_Light::sDoAction(const Action& action)
{
	auto& input = light()->get<CInput>();

	if (action.type() == "START")
	{
		if (action.name() == "UP") { input.up = true; }
		else if (action.name() == "DOWN") { input.down = true; }
		else if (action.name() == "LEFT") { input.left = true; }
		else if (action.name() == "RIGHT") { input.right = true; }
		else if (action.name() == "QUIT") { onEnd(); }
	}
	else if (action.type() == "END")
	{
		if (action.name() == "UP") { input.up = false; }
		else if (action.name() == "DOWN") { input.down = false; }
		else if (action.name() == "LEFT") { input.left = false; }
		else if (action.name() == "RIGHT") { input.right = false; }
	}
}

void Scene_Light::sMovement()
{
	auto& transform = light()->get<CTransform>();
	auto& input = light()->get<CInput>();
	Vec2f velocity = Vec2f(0.0f, 0.0f);
	if (input.up) { velocity.y += -1.0f; }
	if (input.down) { velocity.y += 1.0f; }
	if (input.left) { velocity.x += -1.0f; }
	if (input.right) { velocity.x += 1.0f; }

	if (velocity == Vec2f(0.0f, 0.0f))
	{
		transform.velocity = velocity;
	}
	else
	{
		transform.velocity = velocity / velocity.length() * 5;
	}

	transform.pos += transform.velocity;
	light()->get<CCircleShape>().circle.setPosition(transform.pos);
}

void Scene_Light::sCollision()
{
	auto& transform = light()->get<CTransform>();
	float radius = light()->get<CCircleShape>().circle.getRadius();

	// simple collision checking to dont go out of bounds
	if ((transform.pos.x + radius > width()) || (transform.pos.x - radius) < 0)
	{
		transform.pos.x -= transform.velocity.x;
	}

	if ((transform.pos.y + radius > height()) || (transform.pos.y - radius) < 0)
	{
		transform.pos.y -= transform.velocity.y;
	}
}

void Scene_Light::sLighting()
{
	//TODO implement lighting calculations
}

void Scene_Light::sRender()
{
	m_game.window().clear();

	for (auto& polygon : m_entityManager.getEntities("polygon"))
	{
		m_game.window().draw(polygon->get<CPolygon>().polygon);
	}

	// draw light source
	m_game.window().draw(light()->get<CCircleShape>().circle);

	m_game.window().display();
}

std::shared_ptr<Entity> Scene_Light::light()
{
	auto& lights = m_entityManager.getEntities("light");
	return lights.front();
}