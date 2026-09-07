#pragma once
#include "Action.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Window/Keyboard.hpp>

class GameEngine;

using ActionMap = std::map<sf::Keyboard::Key, std::string>;

class Scene {
protected:
  GameEngine *m_game = nullptr;
  EntityManager m_entities;
  ActionMap m_actionMap;
  size_t m_frame = 0;
  bool m_paused = false;

  Scene() = default;
  explicit Scene(GameEngine *gameEngine) : m_game(gameEngine) {}

  void registerAction(sf::Keyboard::Key code, std::string name) {
    m_actionMap.emplace(code, std::move(name));
  }

  virtual void sDoAction(const Action &action) = 0;

  virtual void sRender() = 0;

  void sAnimation() {
    for (auto &e : m_entities.getEntities()) {
      if (!e->has<CAnimation>() || !e->has<CSprite>()) {
        continue;
      }

      auto &a = e->get<CAnimation>().animation;
      if (!a.loop && a.hasEnded()) {
        continue;
      }

      a.currentFrame = a.loop ? (a.currentFrame + 1) % (a.frameCount * a.speed)
                              : a.currentFrame + 1;
      int animFrame = std::min(a.currentFrame / a.speed, a.frameCount - 1);
      e->get<CSprite>().sprite->setTextureRect(
          sf::IntRect({animFrame * a.size.x, 0}, a.size));
    }
  }

public:
  ~Scene() = default;

  virtual void update() = 0;

  void simulate(const size_t frames) {
    for (size_t i = 0; i < frames; i++) {
      update();
    }
  }

  void doAction(const Action &action) { sDoAction(action); }

  void setPaused(bool paused) { m_paused = paused; }

  size_t frame() const { return m_frame; }
  const ActionMap &actionMap() const { return m_actionMap; }
};
