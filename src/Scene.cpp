#include "Scene.h"
#include "GameEngine.h"

Scene::Scene(GameEngine& gameEngine)
    : m_game(gameEngine) {}

void Scene::registerAction(int inputKey, const std::string &actionName)
{
    m_actionMap[inputKey] = actionName;
}

size_t Scene::width() const
{
    return m_game.width();
}

size_t Scene::height() const
{
    return m_game.height();
}

const ActionMap& Scene::getActionMap() const
{
    return m_actionMap;
}

void Scene::setPaused(bool paused)
{
    m_paused = paused;
}

void Scene::doAction(const Action& action)
{
    sDoAction(action);
}

size_t Scene::currentFrame() const
{
    return m_currentFrame;
}

void Scene::drawLine(const Vec2f& p1, const Vec2f& p2)
{
    sf::Vertex line[] = { sf::Vector2f(p1.x, p1.y), sf::Vector2f(p2.x, p2.y) };
    m_game.window().draw(line, 2, sf::Lines); 
}
