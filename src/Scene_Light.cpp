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

}

void Scene_Light::sDoAction(const Action& action)
{
	//TODO handle user input
}

void Scene_Light::sMovement()
{
	//TODO implement movement so we can move lightsource around
}

void Scene_Light::sLighting()
{
	//TODO implement lighting calculations
}

void Scene_Light::sRender()
{
	m_game.window().clear();

	// TODO: implement all the drawing of polygons and lighting here

	m_game.window().display();

}