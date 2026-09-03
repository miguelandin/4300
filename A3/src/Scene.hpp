#pragma once
#include "Action.hpp"
#include "EntityManager.hpp"
#include <SFML/Window/Keyboard.hpp>

class GameEngine;

using actionMap = std::map<int, std::string>;

class Scene {
protected:
  GameEngine *m_game = nullptr;
  EntityManager m_entities;
  actionMap m_actionMap;
  size_t m_frame = 0;
  bool m_paused = false;

  Scene() = default;

  explicit Scene(GameEngine *gameEngine) : m_game(gameEngine) {}

  void registerAction(sf::Keyboard::Scancode code, std::string name) {
    m_actionMap.emplace(static_cast<int>(code), std::move(name));
  }

public:
  ~Scene() = default;

  virtual void update() = 0;

  virtual void sDoAction(const Action &action) = 0;

  virtual void sRender() = 0;

  void simulate(const size_t frames) {
    for (size_t i = 0; i < frames; i++) {
      update();
    }
  }

  void doAction(const Action &action) { sDoAction(action); }

  void setPaused(bool paused) { m_paused = paused; }

  size_t frame() const { return m_frame; }
};
