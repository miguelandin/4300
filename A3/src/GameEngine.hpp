#include "Scene.hpp"
#include <SFML/Window.hpp>
#include <SFML/Window/Window.hpp>

class GameEngine {
  std::map<std::string, Scene> m_scenes;
  sf::Window m_window;
};
