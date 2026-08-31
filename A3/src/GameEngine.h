#pragma once
#include "Assets.hpp"
#include "Scene.hpp"
#include <SFML/Graphics/RenderWindow.hpp>
#include <memory>
#include <string>

using scene_ptr = std::shared_ptr<Scene>;
using scene_map = std::map<std::string, scene_ptr>;

class GameEngine {
  scene_map m_scenes;
  sf::RenderWindow m_window;
  Assets m_assets;
  std::string m_scene;
  bool m_running;

  void init(const std::string &path);

  scene_ptr currentScene();

public:
  GameEngine(const std::string &path);
  void run();
  void update();
  void quit();
  void setCurrentScene(std::string scene);
  void addScene(std::string sceneName, scene_ptr scene);
  void endActualScene();
  void changeScene(std::string sceneName, scene_ptr scene,
                   bool endScene = false);

  const Assets &assets() const;
  sf::RenderWindow &window();
};
