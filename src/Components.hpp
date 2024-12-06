#pragma once

#include "Vec2.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

class Component
{
public:
    bool exists = false;
};

class CTransform : public Component
{
public:
    Vec2f pos       = { 0.0f, 0.0f };
    Vec2f velocity  = { 0.0f, 0.0f };
    float angle     = 0.0f;

    CTransform() = default;
    CTransform(const Vec2f & p)
        : pos(p) {}
    CTransform(const Vec2f& p, const Vec2f & v)
        : pos(p), velocity(v) {}
    CTransform(const Vec2f & p, const Vec2f & v, float a)
        : pos(p), velocity(v), angle(a) {}
};

class CPolygon : public Component
{
public:

    sf::ConvexShape polygon;

    CPolygon() = default;
    CPolygon(size_t points)
        : polygon(points)
    {
        polygon.setFillColor(sf::Color::Black);
        polygon.setOutlineColor(sf::Color::White);
        polygon.setOutlineThickness(2);
    }
};

class CCircleShape : public Component
{
public:
    sf::CircleShape circle;

    CCircleShape() = default;
    CCircleShape(float radius, size_t points, const sf::Color& fill)
        : circle(radius, points)
    {
        circle.setFillColor(fill);
        circle.setOrigin(radius, radius);
    }
};

class CInput : public Component
{
public:
    bool up         = false;
    bool left       = false;
    bool right      = false;
    bool down       = false;

    CInput() = default;
};