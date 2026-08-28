#pragma once
#include "Assets.hpp"
#include "Scene.hpp"
#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>
#include <string>

class GameEngine {
  std::map<std::string, Scene> m_scenes;
  sf::Window m_window;
  Assets m_assets;
  std::string m_scene;
  bool m_running;
};
