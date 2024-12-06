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
	light->add<CCircleShape>(16, 24, sf::Color(247, 247, 111));
	light->add<CInput>();
}

void Scene_Light::spawnPolygons()
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
		// capping the speed in all directions to 5
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
	for (auto& polygon : m_entityManager.getEntities("polygon"))
	{
		racycastToPolygons(light(), polygon);
	}
}

void Scene_Light::racycastToPolygons(std::shared_ptr<Entity> lightSource, std::shared_ptr<Entity> polygon)
{
	 Vec2f& a = lightSource->get<CTransform>().pos;

	 size_t vertices = polygon->get<CPolygon>().polygon.getPointCount();

	 // raycast to polygon vertices. For each vertex 3 rays should be cast.
	 // one to the vertex, one slightly to the left and one slightly to the right
	 // should check the closest interesect with a polygon segment
	 for (int i = 0; i < vertices; i++)
	 {
		 const Vec2f& vertex = polygon->get<CPolygon>().polygon.getPoint(i);
		 Intersect vertexIntersect = intersectPolygons(a, vertex);
		 Intersect leftIntersect = intersectPolygons(a, rotateLineSegment(a, vertex, 0.0001f));
		 Intersect rightIntersect = intersectPolygons(a, rotateLineSegment(a, vertex, -0.0001f));

		 if (m_drawRays)
		 {
			 drawLine(a, vertexIntersect.pos, sf::Color::Red);
			 drawPoint(vertexIntersect.pos, sf::Color::Red);
			 drawLine(a, leftIntersect.pos, sf::Color::Red);
			 drawPoint(leftIntersect.pos, sf::Color::Red);
			 drawLine(a, rightIntersect.pos, sf::Color::Red);
			 drawPoint(rightIntersect.pos, sf::Color::Red);
		 }
	 }
}

Scene_Light::Intersect Scene_Light::intersectPolygons(const Vec2f& a, const Vec2f& b)
{
	// fake intersect initialized with a very far point so that it gets replaced
	Intersect closestIntersect{ true, Vec2f(width() * 2 + 1, height() * 2 + 1) };

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
			if (intersect.result && (a.dist(intersect.pos) < a.dist(closestIntersect.pos)))
			{
				closestIntersect = intersect;
			}
		}
	}
	return closestIntersect;
}

Vec2f Scene_Light::rotateLineSegment(const Vec2f& a, const Vec2f& b, float angle)
{
	Vec2f dv = b - a;
	Vec2f rotatedVector = Vec2f(dv.x * cos(angle) - dv.y * sin(angle), dv.x * sin(angle) + dv.y * cos(angle));

	// scale factor based on the diagonal of the window to make sure is out of bounds.
	// probably there is a smarter way to do this but it is what it is.
	float scale = Vec2f(0.0f, 0.0f).dist2(Vec2f(width(), height())) / a.dist2(b);
	// scale it so that is out of bounds
	return a + rotatedVector * scale;
}

void Scene_Light::drawLinesToVertices(std::shared_ptr<Entity> polygon)
{
	size_t vertices = polygon->get<CPolygon>().polygon.getPointCount();

	for (int i = 0; i < vertices; i++)
	{
		const Vec2f& vertex = polygon->get<CPolygon>().polygon.getPoint(i);
		drawPoint(vertex, sf::Color::Red);
		drawLine(light()->get<CTransform>().pos, vertex, sf::Color::Red);
	}
}

void Scene_Light::drawPoint(const Vec2f& p, const sf::Color& color)
{
	sf::CircleShape point(4, 8);
	point.setFillColor(color);
	point.setOutlineColor(sf::Color(color.r, color.g, color.b, 100));
	point.setOutlineThickness(4);
	point.setOrigin(4, 4);
	point.setPosition(sf::Vector2f(p.x, p.y));
	m_game.window().draw(point);
}

void Scene_Light::sRender()
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

std::shared_ptr<Entity> Scene_Light::light()
{
	auto& lights = m_entityManager.getEntities("light");
	return lights.front();
}

Scene_Light::Intersect Scene_Light::lineIntersect(const Vec2f& a, const Vec2f& b, const Vec2f& c, const Vec2f& d)
{
	Vec2f r = b - a;
	Vec2f s = d - c;
	float rxs = r.crossProduct(s);
	Vec2f cma = c - a;
	float t = cma.crossProduct(s) / rxs;
	float u = cma.crossProduct(r) / rxs;
	if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
	{
		return { true, Vec2f(a.x + t * r.x, a.y + t * r.y) };
	}
	else
	{
		return { false, Vec2f(0.0f, 0.0f) };
	}
}