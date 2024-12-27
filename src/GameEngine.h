#pragma once

#include "Scene.h"
#include <SFML/Graphics.hpp>
#include <memory>

class Scene;

using SceneMap = std::map<std::string, std::shared_ptr<Scene>>;

class GameEngine
{

protected:

    sf::RenderWindow        m_window;
    std::string             m_currentScene;
    SceneMap                m_sceneMap;
    sf::Clock               m_deltaClock;
    bool                    m_running =  true;

    void                    init();
    void                    update();
    void                    sUserInput();
    std::shared_ptr<Scene>  currentScene();

public:

    GameEngine();

    void                    changeScene(const std::string & sceneName, const std::shared_ptr<Scene> scene, bool endCurrentScene = false);
    void                    quit();
    void                    run();

    sf::RenderWindow &      window();
    sf::Clock&              clock();
    size_t                  width() const;
    size_t                  height() const;
    bool                    isRunning() const;
};