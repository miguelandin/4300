#include "Scene_Play.h"
#include "Action.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"
#include "Scene.hpp"
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <cassert>
#include <iostream>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
  init(levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Key::P, "pause");
  registerAction(sf::Keyboard::Key::Escape, "quit");
  registerAction(sf::Keyboard::Key::T, "toggle_texture");
  registerAction(sf::Keyboard::Key::C, "toggle_collision");
  registerAction(sf::Keyboard::Key::G, "toggle_grid");

  registerAction(sf::Keyboard::Key::Up, "jump");
  registerAction(sf::Keyboard::Key::Space, "jump");
  registerAction(sf::Keyboard::Key::W, "jump");

  registerAction(sf::Keyboard::Key::Left, "left");
  registerAction(sf::Keyboard::Key::A, "left");

  registerAction(sf::Keyboard::Key::Right, "right");
  registerAction(sf::Keyboard::Key::D, "right");

  registerAction(sf::Keyboard::Key::Down, "down");
  registerAction(sf::Keyboard::Key::S, "down");

  registerAction(sf::Keyboard::Key::C, "shoot");
  registerAction(sf::Keyboard::Key::LShift, "shoot");

  loadLevel(levelPath);
}

sf::Vector2f Scene_Play::gridToMidPixel(float gridX, float gridY,
                                        entity_ptr entity) {
  const auto &animation = entity->get<CAnimation>();
  assert(animation.exists);

  auto size = animation.animation->size();
  return sf::Vector2f((m_gridSize.x * gridX) + (size.x / 2.0f),
                      m_game->window().getSize().y -
                          ((m_gridSize.y * gridY) + (size.y / 2.0f)));
}

void Scene_Play::loadLevel(const std::string &filename) {
  // reset the entity manager every time we load the level
  m_entities = EntityManager();

  // TODO read in the level file and add appropiate entities
  // use the playerConfig struct m_playerConfig to store player properties

  // NOTE: all the code below is sample code which show how to set up and use
  // entities, it should be removed
  spawnPlayer();

  auto brick = m_entities.addEntity("tile");
  brick->add<CAnimation>(m_game->assets().getAnimation("question_block"));
  brick->add<CTransform>(gridToMidPixel(0, 0, brick));
  // NOTE: Your final code should position the entity using the gridToMidPixel
  // func read from the levelFile
  // brick->add<CTransform>(gridToMidPixel(gridX,gridY,brick));
  brick->add<CBoundingBox>(
      m_game->assets().getAnimation("question_block").size());
  if (brick->get<CAnimation>().animation->name() == "question_block") {
    // this is a good way for identifying if a tile is a brick!
  }

  auto bush = m_entities.addEntity("bush");
  bush->add<CAnimation>(m_game->assets().getAnimation("bush"));
  bush->add<CTransform>(gridToMidPixel(10, 0, bush));
}

void Scene_Play::spawnPlayer() {
  if (!m_player) {
    m_player = m_entities.addEntity("Player");
  }

  m_player->add<CAnimation>(m_game->assets().getAnimation("stand"));
  m_player->add<CTransform>(gridToMidPixel(0, 0, m_player),
                            sf::Vector2f(7.5f, 7.5f), sf::Vector2f(1.0f, 1.0f),
                            0.0f);
  m_player->add<CBoundingBox>(sf::Vector2f(48, 48));
  m_player->add<CState>("stand");
  m_player->add<CInput>();
  // TODO be sure to add the remaining components to the player (read from
  // m_playerConfig)
}

void Scene_Play::spawnBullet(entity_ptr entity) {
  // TODO spawn a bullet at the given entity, going the direction the entity is
  // facing
}

void Scene_Play::update() {
  m_entities.update();
  // TODO implement pause functionality

  sMovement();
  sLifeSpan();
  sCollision();
  sState();
  sAnimation();
  sRender();
}

void Scene_Play::sMovement() {
  auto &input = m_player->get<CInput>();
  auto &transform = m_player->get<CTransform>();
  auto &state = m_player->get<CState>();
  assert(input.exists && transform.exists && state.exists);

  sf::Vector2f dir;
  if (input.right) {
    dir.x += 1.0f;
  }
  if (input.left) {
    dir.x -= 1.0f;
  }
  if (input.up) {
    dir.y += -1.0f;
  }
  if (input.down) {
    dir.y += 1.0f;
  }

  if (dir != sf::Vector2f(0, 0)) {
    state.state = "run";
    if (dir.x < 0) { // TODO mejorar
      transform.scale.x = -1.0f;
    } else {
      transform.scale.x = 1.0f;
    }
    transform.pos.x += dir.x * transform.velocity.x;
    transform.pos.y += dir.y * transform.velocity.y;
  } else {
    state.state = "stand";
  }

  // TODO Implement player movement/jumping based on its CInput component
  // TODO Implement gravity's effect on the player
  // TODO Implement the maximum player speed in booth X and Y directions
  // NOTE: Settings an entity's scale.x to -1/1 will make it face to the
  // left/right
}

void Scene_Play::sLifeSpan() {
  // TODO same as A2
}
void Scene_Play::sCollision() {
  // REMEMBER: SFML (0,0) position is on the TOP LEFT CORNER this means
  // jumping will have a positive y-component Also, something BELOW something
  // else will have a y value GREATER than it Also, something ABOVE something
  // else will have a y value LESS than it
  // TODO: Implement Physics::GetOverlap() function, use it inside this
  // function
  // TODO: Implement bullet / tile collisions
  //       destroy the tile if it has a Brick animation
  // TODO: Implement player / tile collisions and resolutions update the
  // CState component of the player to store wether it is currently on the
  // ground or in the air. this will be used by the animatin system
  // TODO: Check to see if the player has fallen down a hole (y > height())
  // TODO: Don't let the player walk of the left side of the map
}
void Scene_Play::sAnimation() {
  for (auto &e : m_entities.getEntities()) {
    if (e->has<CAnimation>()) {
      e->get<CAnimation>().animation->update();
    }
  }
}

void Scene_Play::sRender() {
  m_game->window().clear({255, 255, 255});
  for (auto &e : m_entities.getEntities()) {
    if (e->has<CAnimation>() && e->has<CTransform>()) {
      auto sprite = e->get<CAnimation>().animation->sprite();
      sprite.scale(e->get<CTransform>().scale);
      auto pos = e->get<CTransform>().pos;
      sprite.setPosition({std::round(pos.x), std::round(pos.y)});
      m_game->window().draw(sprite);
    }
  }
}

void Scene_Play::sState() {
  auto &animation = m_player->get<CAnimation>();
  auto &state = m_player->get<CState>();
  assert(animation.exists && state.exists);

  if (state.state != animation.animation->name()) {
    m_player->add<CAnimation>(m_game->assets().getAnimation(state.state));
  }
}

void Scene_Play::sDoAction(const Action &action) {
  auto &input = m_player->get<CInput>();
  assert(input.exists);
  bool type = action.type();
  if (action.name() == "jump") {
    input.up = type;
  } else if (action.name() == "right") {
    input.right = type;
  } else if (action.name() == "left") {
    input.left = type;
  } else if (action.name() == "down") {
    input.down = type;
  } else if (action.name() == "shoot") {
    input.shoot = type;
  }
}
