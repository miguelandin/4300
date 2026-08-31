#include "GameEngine.h"
#include "Scene_Play.h"
#include <SFML/Graphics/RenderWindow.hpp>

void GameEngine::init(const std::string &path) {
  m_window.create(sf::VideoMode({1280, 960}), "Not Mario");
  m_window.setFramerateLimit(60);
  m_assets.loadFromFile(path);
  m_running = true;

  changeScene("LEVEL1", std::make_shared<Scene_Play>(this, "NA"));
}

scene_ptr GameEngine::currentScene() { return m_scenes.at(m_scene); }

GameEngine::GameEngine(const std::string &path) { init(path); }

void GameEngine::run() {
  while (m_running) {
    update();
    m_window.display();
  }
}

void GameEngine::update() { currentScene()->update(); }

void GameEngine::quit() {
  m_running = false;
  m_window.close();
};

void GameEngine::setCurrentScene(std::string scene) {
  m_scene = std::move(scene);
}

void GameEngine::addScene(std::string sceneName, scene_ptr scene) {
  m_scenes.insert_or_assign(std::move(sceneName), std::move(scene));
}

void GameEngine::endActualScene() { m_scenes.erase(m_scene); }

void GameEngine::changeScene(std::string sceneName, scene_ptr scene,
                             bool endScene) {
  if (endScene) {
    endActualScene();
  }
  if (scene) {
    addScene(sceneName, std::move(scene));
  }
  setCurrentScene(std::move(sceneName));
}

const Assets &GameEngine::assets() const { return m_assets; }

sf::RenderWindow &GameEngine::window() { return m_window; }
