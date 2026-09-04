#pragma once
#include "Action.hpp"
#include "EntityManager.hpp"
#include "GameEngine.h"
#include "Scene.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

class Scene_Play : public Scene {
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY;
    std::string weapon;
  };

protected:
  entity_ptr m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  bool m_drawTextures = true;
  bool m_drawCollision = false;
  bool m_drawGrid = false;
  const sf::Vector2f m_gridSize = {64, 64};

  void init(const std::string &levelPath);
  void loadLevel(const std::string &filename);
  sf::Vector2f gridToMidPixel(float gridX, float gridY, entity_ptr entity);
  void spawnPlayer();
  void spawnBullet(entity_ptr entity);

  void sMovement();
  void sLifeSpan();
  void sCollision();
  void sAnimation();
  void sRender() override;
  void sDoAction(const Action &action) override;
  void sState();

public:
  Scene_Play(GameEngine *gameEngine, const std::string &levelPath);

  void update() override;
};
