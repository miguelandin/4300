#pragma once

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "EntityManager.hpp"
#include "imgui/imgui-SFML.h"
#include "imgui/imgui.h"

struct WindowConfig
{
  unsigned int W, H;
  int FL;
  bool FS;
};

struct FontConfig
{
  std::string F;
  int S, R, G, B;
};

struct PlayerConfig
{
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, FF, SC, L;
  float S, A, F;
};

struct EnemyConfig
{
  int SR, CR, OR, OG, OB, OT, VMIN, VMAX, L, SI, M, T;
  float SMIN, SMAX;
};

struct BulletConfig
{
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S, I;
};

struct SpecialConfig
{
  int SR, CR, FR, FG, FB, OR, OG, OB, OT, V, L;
  float S, I;
};

enum lyr : uint8_t
{
  NONE = 0,
  PLAYER = 1 << 0,
  ENEMY = 1 << 1,
  P_BULLET = 1 << 2,
  E_BULLET = 1 << 3,
  P_SMALL = 1 << 4,
  E_SMALL = 1 << 5
};

class Game
{
  sf::RenderWindow m_window;
  EntityManager m_entities;
  sf::Clock m_deltaClock;
  int m_score = 0;
  int m_currentFrame = 0;
  int m_lastEnemySpawnTime = 0;
  int m_lastSpecialEnemySpawnTime = 0;
  bool m_paused = false;
  WindowConfig m_wCf;
  FontConfig m_fCf;
  PlayerConfig m_pCf;
  EnemyConfig m_eCf;
  BulletConfig m_bCf;
  SpecialConfig m_sCf;
  sf::Font m_font;
  sf::Text m_text;

  void init(const std::string &config); // read config file
  void setPaused(bool paused);          // pause the game

  /* SYSTEMS */
  void sMovement();
  void sUserInput();
  void sLifespan();
  void sRender();
  void sGUI();
  void sEnemySpawner();
  void sCollision();
  void sShooting();
  void sInterface();

  void spawnPlayer();

  std::shared_ptr<Entity> spawnEnemy(size_t points, const sf::Color &fill, const Vec2f &p, const Vec2f &v, float angle);

  void spawnExplosion(std::shared_ptr<Entity> entity);

  void spawnBullet(std::shared_ptr<Entity> entity, const Vec2f &mousePos, lyr type);

  void spawnSpecialWeapon(std::shared_ptr<Entity> entity, const Vec2f &target, lyr ty);

  std::shared_ptr<Entity> player();

  bool isColliding(const Vec2f &p1, const Vec2f &p2, float r1, float r2);

  bool isColliding(const std::shared_ptr<Entity> &e1, const std::shared_ptr<Entity> &e2);

  void manageCollision(std::shared_ptr<Entity> entity);

public:
  Game(const std::string &config);
  void run();
};
