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

class CInput : public Component
{
public:
    bool up         = false;
    bool left       = false;
    bool right      = false;
    bool down       = false;

    CInput() = default;
};