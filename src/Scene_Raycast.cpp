#include "Scene_Raycast.h"
#include "Scene_Light.h"
#include "GameEngine.h"
#include "Components.hpp"
#include "Action.hpp"

#include <iostream>
#include <cmath>
#include <vector>
#include <map>
#include <algorithm>

Scene_Raycast::Scene_Raycast(GameEngine& gameEngine, Vec2f pos)
	: Scene(gameEngine)
{
	init(pos);
}

void Scene_Raycast::init(Vec2f& pos)
{
	registerAction(sf::Keyboard::Escape, "QUIT");
	registerAction(sf::Keyboard::W, "UP");
	registerAction(sf::Keyboard::A, "LEFT");
	registerAction(sf::Keyboard::S, "DOWN");
	registerAction(sf::Keyboard::D, "RIGHT");
	registerAction(sf::Keyboard::C, "CHANGESCENE");
	registerAction(sf::Keyboard::E, "RAYCAST");

	spawnLightSource(pos);
	spawnPolygons();
}

void Scene_Raycast::update()
{
	m_entityManager.update();

	sMovement();
	sCollision();
	sRender();

	m_currentFrame++;
}

void Scene_Raycast::onEnd()
{
	m_game.quit();
}

void Scene_Raycast::spawnLightSource(Vec2f& pos)
{
	auto light = m_entityManager.addEntity("light");
	light->add<CTransform>(pos);
	light->add<CCircleShape>(16, 24, sf::Color(247, 247, 111));
	light->add<CInput>();
}

void Scene_Raycast::spawnPolygons()
{
	// Hardcoded polygons and bounds, clearly not scalable
	// probably read from file for true implementation into game
	std::map<size_t, std::vector<Vec2f>> polyMap;
	polyMap[0] = std::vector<Vec2f>{ Vec2f(0.0f, 0.0f), Vec2f(0.0f, height()), Vec2f(width(), height()), Vec2f(width(), 0.0f) };
	polyMap[1] = std::vector<Vec2f>{ Vec2f(100.0f, 150.0f), Vec2f(120.0f, 50.0f), Vec2f(200.0f, 80.0f), Vec2f(140.0f, 210.0f) };
	polyMap[2] = std::vector<Vec2f>{ Vec2f(100.0f, 200.0f), Vec2f(120.0f, 250.0f), Vec2f(60.0f, 300.0f) };
	polyMap[3] = std::vector<Vec2f>{ Vec2f(200.0f, 260.0f), Vec2f(220.0f, 150.0f), Vec2f(300.0f, 200.0f), Vec2f(350.0f, 320.0f) };
	polyMap[4] = std::vector<Vec2f>{ Vec2f(540.0f, 60.0f), Vec2f(560.0f, 40.0f), Vec2f(570.0f, 70.0f) };
	polyMap[5] = std::vector<Vec2f>{ Vec2f(650.0f, 190.0f), Vec2f(760.0f, 170.0f), Vec2f(740.0f, 270.0f), Vec2f(630.0f, 290.0f) };
	polyMap[6] = std::vector<Vec2f>{ Vec2f(600.0f, 95.0f), Vec2f(780.0f, 50.0f), Vec2f(680.0f, 150.0f)};
	polyMap[7] = std::vector<Vec2f>{ Vec2f(100.0f, 600.0f), Vec2f(150.0f, 520.0f), Vec2f(200.0f, 620.0f), Vec2f(130.0f, 700.0f) };
	polyMap[8] = std::vector<Vec2f>{ Vec2f(300.0f, 500.0f), Vec2f(400.0f, 450.0f), Vec2f(350.0f, 550.0f) };
	polyMap[9] = std::vector<Vec2f>{ Vec2f(600.0f, 500.0f), Vec2f(700.0f, 470.0f), Vec2f(750.0f, 580.0f), Vec2f(650.0f, 620.0f) };
	polyMap[10] = std::vector<Vec2f>{ Vec2f(900.0f, 300.0f), Vec2f(950.0f, 250.0f), Vec2f(1000.0f, 350.0f), Vec2f(920.0f, 400.0f) };
	polyMap[11] = std::vector<Vec2f>{ Vec2f(1050.0f, 600.0f), Vec2f(1150.0f, 480.0f), Vec2f(1180.0f, 670.0f), Vec2f(1000.0f, 700.0f) };
	polyMap[12] = std::vector<Vec2f>{ Vec2f(400.0f, 300.0f), Vec2f(450.0f, 280.0f), Vec2f(500.0f, 320.0f), Vec2f(430.0f, 350.0f) };
	polyMap[13] = std::vector<Vec2f>{ Vec2f(1050.0f, 40.0f), Vec2f(1200.0f, 180.0f), Vec2f(950.0f, 200.0f), Vec2f(850.0f, 160.0f) };

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

void Scene_Raycast::sDoAction(const Action& action)
{
	auto& input = light()->get<CInput>();

	if (action.type() == "START")
	{
		if (action.name() == "UP") { input.up = true; }
		else if (action.name() == "DOWN") { input.down = true; }
		else if (action.name() == "LEFT") { input.left = true; }
		else if (action.name() == "RIGHT") { input.right = true; }
		else if (action.name() == "RAYCAST") { m_raycast = !m_raycast; }
		else if (action.name() == "QUIT") { onEnd(); }
		else if (action.name() == "CHANGESCENE") 
		{
			m_game.changeScene("LIGHT", std::make_shared<Scene_Light>(m_game, light()->get<CTransform>().pos), true);
		}
	}
	else if (action.type() == "END")
	{
		if (action.name() == "UP") { input.up = false; }
		else if (action.name() == "DOWN") { input.down = false; }
		else if (action.name() == "LEFT") { input.left = false; }
		else if (action.name() == "RIGHT") { input.right = false; }
	}
}

void Scene_Raycast::sMovement()
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
		// capping the speed in all directions to 5
		transform.velocity = velocity / velocity.length() * 5;
	}

	transform.pos += transform.velocity;
	light()->get<CCircleShape>().circle.setPosition(transform.pos);
}

void Scene_Raycast::sCollision()
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

void Scene_Raycast::sLighting()
{
	std::vector<Intersect> allIntersects;

	for (auto& polygon : m_entityManager.getEntities("polygon"))
	{
		std::vector<Intersect> intersects = racycastToPolygons(light(), polygon);
		allIntersects.insert(allIntersects.end(), intersects.begin(), intersects.end());
	}

	if (m_raycast)
	{
		for (size_t i = 0; i < allIntersects.size(); i++)
		{
			drawLine(light()->get<CTransform>().pos, allIntersects[i].pos, sf::Color::Red);
			drawPoint(allIntersects[i].pos, sf::Color::Red);
		}
	}
}

std::vector<Scene_Raycast::Intersect> Scene_Raycast::racycastToPolygons(std::shared_ptr<Entity> lightSource, std::shared_ptr<Entity> polygon)
{
	Vec2f& a = lightSource->get<CTransform>().pos;

	size_t vertices = polygon->get<CPolygon>().polygon.getPointCount();
	std::vector<Intersect> intersects;

	// raycast to polygon vertices. For each vertex cast 2 rays.
	// one slightly to the left and one slightly to the right of the vertex
	// these left and right make sure lighting reaches the bounds of the window
	// should check the closest interesect with a polygon segment and return it
	for (int i = 0; i < vertices; i++)
	{
		const Vec2f& vertex = polygon->get<CPolygon>().polygon.getPoint(i);
		Intersect vertexIntersect = intersectPolygons(a, vertex);
		Intersect leftIntersect = intersectPolygons(a, rotateLineSegment(a, vertex, 0.0001f));
		Intersect rightIntersect = intersectPolygons(a, rotateLineSegment(a, vertex, -0.0001f));

		intersects.push_back(leftIntersect);
		intersects.push_back(rightIntersect);
	}
	return intersects;
}

Scene_Raycast::Intersect Scene_Raycast::intersectPolygons(const Vec2f& a, const Vec2f& b)
{
	// fake intersect initialized with a very far point so that it gets replaced
	Intersect closestIntersect{ true, Vec2f(width() * 2 + 1, height() * 2 + 1), 0.0f };

	for (auto& otherPolygon : m_entityManager.getEntities("polygon"))
	{
		size_t v = otherPolygon->get<CPolygon>().polygon.getPointCount();
		for (int j = 0; j < v; j++)
		{
			const Vec2f& c = otherPolygon->get<CPolygon>().polygon.getPoint(j);
			// (j+1)%v so that when we get to the last point we loop to the first to close the polygon
			const Vec2f& d = otherPolygon->get<CPolygon>().polygon.getPoint((j + 1) % v);

			// now we have the 2 segments to check intersection (a-b) and (c-d)
			Intersect intersect = lineIntersect(a, b, c, d);
			// if it intersects and the intersection point is the closest then replace closest intersect
			if (intersect.result && (a.dist2(intersect.pos) < a.dist2(closestIntersect.pos)))
			{
				closestIntersect = intersect;
			}
		}
	}
	return closestIntersect;
}

Vec2f Scene_Raycast::rotateLineSegment(const Vec2f& a, const Vec2f& b, float angle)
{
	Vec2f dv = b - a;
	Vec2f rotatedVector = Vec2f(dv.x * cos(angle) - dv.y * sin(angle), dv.x * sin(angle) + dv.y * cos(angle));

	// scale factor based on the diagonal of the window to make sure is out of bounds.
	// probably there is a smarter way to do this but it is what it is.
	float scale = Vec2f(0.0f, 0.0f).dist2(Vec2f(width(), height())) / a.dist2(b);
	// scale it so that is out of bounds
	return a + rotatedVector * scale;
}

void Scene_Raycast::drawPoint(const Vec2f& p, const sf::Color& color)
{
	sf::CircleShape point(4, 8);
	point.setFillColor(color);
	point.setOutlineColor(sf::Color(color.r, color.g, color.b, 100));
	point.setOutlineThickness(4);
	point.setOrigin(4, 4);
	point.setPosition(sf::Vector2f(p.x, p.y));
	m_game.window().draw(point);
}

void Scene_Raycast::sRender()
{
	m_game.window().clear();


	for (auto& polygon : m_entityManager.getEntities("polygon"))
	{
		m_game.window().draw(polygon->get<CPolygon>().polygon);
	}

	sLighting();

	// draw light source
	m_game.window().draw(light()->get<CCircleShape>().circle);

	m_game.window().display();
}

std::shared_ptr<Entity> Scene_Raycast::light()
{
	auto& lights = m_entityManager.getEntities("light");
	return lights.front();
}

Scene_Raycast::Intersect Scene_Raycast::lineIntersect(const Vec2f& a, const Vec2f& b, const Vec2f& c, const Vec2f& d)
{
	Vec2f r = b - a;
	Vec2f s = d - c;
	float rxs = r.crossProduct(s);
	Vec2f cma = c - a;
	float t = cma.crossProduct(s) / rxs;
	float u = cma.crossProduct(r) / rxs;
	Vec2f point = Vec2f(a.x + t * r.x, a.y + t * r.y);
	float angle = atan2(point.y - a.y, point.x - a.x);
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, point,  angle };
	}
	else
	{
		return { false, Vec2f(0.0f, 0.0f), 0 };
	}
}