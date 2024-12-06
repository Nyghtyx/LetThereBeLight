#pragma once

#include "Action.hpp"
#include "EntityManager.hpp"

#include <memory>
#include <map>

class GameEngine;

using ActionMap = std::map<int, std::string>; // input key, action name

class Scene
{

protected:

    GameEngine&             m_game;
    EntityManager           m_entityManager;
    size_t                  m_currentFrame = 0;
    ActionMap               m_actionMap;
    bool                    m_paused = false;
    bool                    m_hasEnded = false;

    Scene(GameEngine& game);
    virtual void onEnd() = 0;
    void setPaused(bool paused);

public:
    
    virtual void update()                           = 0;
    virtual void sDoAction(const Action& action)    = 0;
    virtual void sRender()                          = 0;

    const ActionMap& getActionMap() const;

    void doAction(const Action & action);
    void registerAction(int inputKey, const std::string& actionName);
    void drawLine(const Vec2f& p1, const Vec2f& p2, const sf::Color& color = { 255, 255, 255 });

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;
};