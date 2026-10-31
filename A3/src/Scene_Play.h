#pragma once
#include "Action.hpp"
#include "Assets.hpp"
#include "EntityManager.hpp"
#include "GameEngine.h"
#include "Scene.hpp"
#include <SFML/Graphics/Vertex.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <string>

class Scene_Play : public Scene {
  struct PlayerConfig {
    float X, Y, CX, CY, SPEED = 2.0f, MAX_X_SPEED = 10.0f, MAX_Y_SPEED = 64.0f,
                        JUMP = -26.0f, GRAVITY, FRICTION = 0.9f;
    int LANDING_DURATION =
            Assets::instance().getAnimation("land").getFrameLength(),
        CROUCHING_DURATION =
            Assets::instance().getAnimation("crouch").getFrameLength(),
        SLIDING_DURATION =
            Assets::instance().getAnimation("slide").getFrameLength(),
        JUMP_KICK_DURATION =
            Assets::instance().getAnimation("jump_kick").getFrameLength(),
        CUSHIONING_DURATION =
            Assets::instance().getAnimation("cushion").getFrameLength();
    std::string weapon;
  };

protected:
  entity_ptr m_player;
  std::string m_levelPath;
  PlayerConfig m_playerConfig;
  bool m_drawTextures = true;
  bool m_drawBoundingBox = false;
  bool m_drawGrid = false;
  const sf::Vector2f m_gridSize = {64, 64};
  sf::View m_gameCamera{sf::Vector2f{0.f, 0.f},sf::Vector2f{m_game->window().getSize()}};

  void init(const std::string &levelPath);
  void loadLevel(const std::string &filename);
  sf::Vector2f gridToMidPixel(float gridX, float gridY, entity_ptr entity);
  void spawnPlayer();
  void spawnBullet(const entity_ptr &entity);

  void sMovement();
  void sLifeSpan();
  void sCollision();
  void sState();
  void sAnimationState();
  void sBoundingBoxState();
  void sRender() override;
  void sCamera();
  void sDoAction(const Action &action) override;

  void drawLines(std::span<const sf::Vertex> points);
  void drawLineStrip(std::span<const sf::Vertex> points);
  void drawBoundingBox(const entity_ptr &entity);
  void drawGrid();
  void drawTexture(const entity_ptr &entity);

public:
  Scene_Play(GameEngine *gameEngine, const std::string &levelPath);

  void update() override;
};
