#include "Scene_Light.h"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>
#include <cmath>

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
}

void Scene_Light::update()
{
	m_entityManager.update();

	sMovement();
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

void Scene_Light::sLighting()
{
	//TODO implement lighting calculations
}

void Scene_Light::sRender()
{
	m_game.window().clear();

	// draw light
	m_game.window().draw(light()->get<CCircleShape>().circle);

	m_game.window().display();

}

std::shared_ptr<Entity> Scene_Light::light()
{
	auto& lights = m_entityManager.getEntities("light");
	return lights.front();
}