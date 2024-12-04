#include "Scene.h"
#include "Scene_Light.h"
#include "GameEngine.h"

#include <iostream>

GameEngine::GameEngine()
{
    init();
}

void GameEngine::init()
{
    m_window.create(sf::VideoMode(1280, 768), "Let There Be Light");
    m_window.setFramerateLimit(60);

    changeScene("LIGHT", std::make_shared<Scene_Light>(*this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    return m_sceneMap.at(m_currentScene);
}

bool GameEngine::isRunning() const
{
    return m_running && m_window.isOpen();
}

sf::RenderWindow & GameEngine::window()
{
    return m_window;
}

size_t GameEngine::width() const
{
    return m_window.getSize().x;
}

size_t GameEngine::height() const
{
    return m_window.getSize().y;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::update()
{
    sUserInput();
    currentScene()->update();
}

void GameEngine::quit()
{
    m_running = false;
}

void GameEngine::sUserInput()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                std::cout << "screenshot saved to " << "text.png" << std::endl;
                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            // if current scene does not have an action associated with this key, skip the event
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }

            // determine start or end action by whether it was key press or key release
            const std::string actionType = (event.type == sf::Event::KeyPressed) ? "START" : "END";

            // look up the action and send the action to the scene
            currentScene()->doAction(Action(currentScene()->getActionMap().at(event.key.code), actionType));
        }
    }
}

void GameEngine::changeScene(const std::string& sceneName, const std::shared_ptr<Scene> scene, bool endCurrentScene)
{    
    if (endCurrentScene)
    {
        auto it = m_sceneMap.find(m_currentScene);
        if (it != m_sceneMap.end()) { m_sceneMap.erase(it); }
    }

    if (m_sceneMap.find(sceneName) == m_sceneMap.end())
    {
        m_sceneMap[sceneName] = scene;
    }

    m_currentScene = sceneName;
}