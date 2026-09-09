#include "Scene_Play.h"
#include "Action.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <vector>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
  init(levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Key::P, "pause");
  registerAction(sf::Keyboard::Key::Escape, "quit");
  registerAction(sf::Keyboard::Key::T, "toggle_texture");
  registerAction(sf::Keyboard::Key::G, "toggle_grid");
  registerAction(sf::Keyboard::Key::B, "toggle_collision");

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
  const auto &cSprite = entity->get<CSprite>();
  assert(cSprite.exists);

  auto size = cSprite.sprite->getTextureRect().size;
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
  auto animation =
      brick->add<CAnimation>(m_game->assets().getAnimation("question_block"));
  auto sprite = brick->add<CSprite>(animation);
  brick->add<CBoundingBox>(sprite);
  brick->add<CTransform>(gridToMidPixel(5, 5, brick));
  // NOTE: Your final code should position the entity using the gridToMidPixel
  // func read from the levelFile
  // brick->add<CTransform>(gridToMidPixel(gridX,gridY,brick));
  // if (brick->get<CSprite>().sprite->name() == "question_block") {
  // this is a good way for identifying if a tile is a brick!
  //}

  auto bush = m_entities.addEntity("bush");
  auto animation1 =
      bush->add<CAnimation>(m_game->assets().getAnimation("bush"));
  auto sprite1 = bush->add<CSprite>(animation1);
  bush->add<CTransform>(gridToMidPixel(10, 1, bush));

  auto dirt = m_entities.addEntity("dirt");
  auto &sprite2 = dirt->add<CSprite>(m_game->assets().getTexture("ground"), 20);
  dirt->add<CTransform>(gridToMidPixel(0, 0, dirt));
  dirt->add<CBoundingBox>(sprite2);
}

void Scene_Play::spawnPlayer() {
  if (!m_player) {
    m_player = m_entities.addEntity("Player");
  }

  const auto &state = m_player->add<CState>("stand").state;
  const auto &animation =
      m_player->add<CAnimation>(m_game->assets().getAnimation(state));
  const auto &sprite = m_player->add<CSprite>(animation);
  m_player->add<CBoundingBox>(sprite, 0.9f, 0.9f);
  m_player->add<CTransform>(gridToMidPixel(0, 0, m_player));
  m_player->add<CInput>();
  m_player->add<CGravity>(1.0f);

  // TODO be sure to add the remaining components to the player (read from
  // m_playerConfig)
}

void Scene_Play::spawnBullet(const entity_ptr &entity) {
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
  auto &gravity = m_player->get<CGravity>();
  assert(input.exists && transform.exists && state.exists && gravity.exists);

  transform.velocity.x += 2.0f * input.right;
  transform.velocity.x -= 2.0f * input.left;

  if (input.right == input.left) {
    float v = transform.velocity.x * 0.9f;
    transform.velocity.x = v * (std::abs(v) >= 1.0f);
  }

  if (!input.up) {
    input.hasJumped = false;
  }

  input.canJump = gravity.airFrames < 6;
  if (input.up && input.canJump && !input.hasJumped) {
    transform.velocity.y = -26.0f;
    gravity.airFrames = 100;
    input.hasJumped = true;
  }

  transform.velocity.y += gravity.acc;

  if (!input.up && transform.velocity.y < 0.0f) {
    transform.velocity.y *= 0.5f;
  }

  transform.velocity.x = std::clamp(transform.velocity.x, -10.0f, 10.0f);
  transform.velocity.y = std::clamp(transform.velocity.y, -64.0f, 64.0f);

  transform.scale.x = std::copysign(
      transform.scale.x,
      transform.velocity.x != 0.0f ? transform.velocity.x : transform.scale.x);

  transform.prevPos = transform.pos;
  transform.pos += transform.velocity;

  if (state.isGrounded) {
    if (std::abs(transform.velocity.x) < 2.0f) {
      state.state = "stand";
    } else {
      state.state = "run";
    }
  } else {
    if (transform.velocity.y < 0.0f) {
      state.state = "jump";
    } else {
      state.state = "fall";
    }
  }
}

void Scene_Play::sLifeSpan() {
  // TODO same as A2
}

void Scene_Play::sCollision() {
  auto &state = m_player->get<CState>();
  auto &transform = m_player->get<CTransform>();
  auto &input = m_player->get<CInput>();
  auto &gravity = m_player->get<CGravity>();
  assert(state.exists && transform.exists && input.exists && gravity.exists);

  gravity.airFrames++;
  state.isGrounded = false;
  for (auto &e : m_entities.getEntities()) {
    if (!e->has<CBoundingBox>() || !e->has<CTransform>() || e == m_player) {
      continue;
    }

    auto &eTransform = e->get<CTransform>();
    auto overlap = Physics::getOverlap(m_player, e);

    if (overlap.x > 0.001f && overlap.y > 0.001f) {
      auto pOverlap = Physics::getPreviousOverlap(m_player, e);

      if (pOverlap.x > pOverlap.y) {
        if (transform.prevPos.y < eTransform.prevPos.y) { // TOP
          transform.pos.y -= overlap.y;
          gravity.airFrames = 0;
          state.isGrounded = true;
        } else { // BOTTOM
          transform.pos.y += overlap.y;
        }
        transform.velocity.y = 0;
      } else {
        if (transform.prevPos.x < eTransform.prevPos.x) { // LEFT
          transform.pos.x -= overlap.x;
        } else { // RIGHT
          transform.pos.x += overlap.x;
        }
        transform.velocity.x = 0;
      }
    }

    // REMEMBER: SFML (0,0) position is on the TOP LEFT CORNER this means
    // jumping will have a positive y-component Also, something BELOW
    // something else will have a y value GREATER than it Also, something
    // ABOVE something else will have a y value LESS than it
    // TODO: Implement Physics::GetOverlap() function, use it inside this
    // function
    // TODO: Implement bullet / tile collisions
    //       destroy the tile if it has a Brick animation
    // TODO: Implement player / tile collisions and resolutions update the
    // CState component of the player to store wether it is currently on the
    // ground or in the air. this will be used by the animatin system
    // TODO: Check to see if the player has fallen down a hole (y >
    // height())
    // TODO: Don't let the player walk of the left side of the map
  }
}

void Scene_Play::sState() {
  auto &animation = m_player->get<CAnimation>();
  auto &state = m_player->get<CState>();
  assert(animation.exists && state.exists);

  if (state.state != animation.animation.name) {
    const auto &animation =
        m_player->add<CAnimation>(m_game->assets().getAnimation(state.state));
    m_player->add<CSprite>(animation);
  }
}

void Scene_Play::sRender() {
  m_game->window().clear({255, 255, 255});

  if (m_drawGrid) {
    drawGrid();
  }

  if (m_drawTextures) {
    for (auto &e : m_entities.getEntities()) {
      if (e != m_player) {
        drawTexture(e);
      }
    }
    drawTexture(m_player);
  }

  if (m_drawCollision) {
    for (auto &e : m_entities.getEntities()) {
      drawCollision(e);
    }
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
  } else if (action.name() == "toggle_collision" && action.type()) {
    m_drawCollision = !m_drawCollision;
  } else if (action.name() == "toggle_texture" && action.type()) {
    m_drawTextures = !m_drawTextures;
  }
}

void Scene_Play::drawLines(std::span<const sf::Vertex> points) {
  assert(!points.empty());
  m_game->window().draw(points.data(), points.size(),
                        sf::PrimitiveType::LineStrip);
}

void Scene_Play::drawCollision(const entity_ptr &e) {
  if (!e->has<CBoundingBox>() || !e->has<CTransform>()) {
    return;
  }
  auto size = e->get<CBoundingBox>().halfSize;
  auto pos = e->get<CTransform>().pos;
  sf::Color color(sf::Color::Red);
  sf::Vertex points[5] = {
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y + size.y), color),
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y - size.y), color),
      sf::Vertex(sf::Vector2f(pos.x + size.x, pos.y - size.y), color),
      sf::Vertex(sf::Vector2f(pos.x + size.x, pos.y + size.y), color),
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y + size.y), color)};

  drawLines(points);
}

void Scene_Play::drawTexture(const entity_ptr &e) {
  if (!e->has<CSprite>() || !e->has<CTransform>()) {
    return;
  }

  auto &sprite = e->get<CSprite>().sprite;
  auto &transform = e->get<CTransform>();
  sprite->setScale(transform.scale);
  sprite->setPosition(
      {std::round(transform.pos.x), std::round(transform.pos.y)});
  m_game->window().draw(*sprite);
}

void Scene_Play::drawGrid() {}
