#pragma once

#include "Vec2.hpp"
#include <SFML/Graphics.hpp>

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
    // TODO: This component should hold the polygon info
    // Check if SFML has some polygon class already
    // If not can specify polygon with the 4 vertices
public:

    CPolygon() = default;
};

class CCircleShape : public Component
{
public:
    sf::CircleShape circle;

    CCircleShape() = default;
    CCircleShape(float radius, size_t points, const sf::Color& fill, const sf::Color& outline, float thickness)
        : circle(radius, points)
    {
        circle.setFillColor(fill);
        circle.setOutlineColor(outline);
        circle.setOutlineThickness(thickness);
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