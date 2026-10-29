#include "Scene_Play.h"
#include "Action.hpp"
#include "Assets.hpp"
#include "Component.hpp"
#include "EntityManager.hpp"
#include "Physics.hpp"
#include "Scene.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/System/Vector2.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <algorithm>
#include <cassert>
#include <cmath>
#include <format>
#include <iostream>
#include <string>
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

  auto brick_1 = m_entities.addEntity("tile");
  auto animation_1 =
      brick_1->add<CAnimation>(m_game->assets().getAnimation("question_block"));
  auto sprite_1 = brick_1->add<CSprite>(animation);
  brick_1->add<CBoundingBox>(sprite);
  brick_1->add<CTransform>(gridToMidPixel(15, 5, brick));
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
  auto &sprite2 =
      dirt->add<CSprite>(m_game->assets().getTexture("ground"), 10000);
  dirt->add<CTransform>(gridToMidPixel(0, 0, dirt));
  dirt->add<CBoundingBox>(sprite2);
}

void Scene_Play::spawnPlayer() {
  if (!m_player) {
    m_player = m_entities.addEntity("Player");
  }

  m_player->add<CBoundingBox>(m_player->add<CSprite>(
      m_player->add<CAnimation>(Assets::instance().getAnimation("stand"))));
  m_player->add<CState>();
  m_player->add<CTransform>(gridToMidPixel(0, 1, m_player));
  m_player->add<CInput>();
  m_player->add<CGravity>(1.0f);
  m_player->add<CStateAnimationMap>(std::unordered_map<State, std::string>{
      {State::Idle, "stand"},
      {State::Running, "run"},
      {State::Jumping, "jump"},
      {State::Falling, "fall"},
      {State::Landing, "land"},
      {State::Crouching, "crouch"},
      {State::Uncrouching, "uncrouch"},
      {State::CrouchIdle, "crouch_idle"},
      {State::CrouchWalking, "crouch_walk"},
      {State::Sliding, "slide"},
      {State::Diving, "dive"},
      {State::Cushioning, "cushion"},
      {State::KickJumping, "jump_kick"},
      {State::FastRunning, "fast_run"}});
  m_player->add<CStateBoundingBoxMap>(
      std::unordered_map<State, StateBoundingBox>{
          {State::Idle, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::Running, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::FastRunning, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::Jumping, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::Falling, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::Landing, {.disp = {0.0f, 8.0f}, .size = {32.0f, 96.0f}}},
          {State::Crouching, {.disp = {0.0f, 32.0f}, .size = {48.0f, 48.0f}}},
          {State::CrouchIdle, {.disp = {0.0f, 32.0f}, .size = {48.0f, 48.0f}}},
          {State::CrouchWalking,
           {.disp = {0.0f, 32.0f}, .size = {48.0f, 48.0f}}},
          {State::Uncrouching, {.disp = {0.0f, 32.0f}, .size = {48.0f, 48.0f}}},
          {State::Sliding, {.disp = {0.0f, 36.0f}, .size = {96.0f, 32.0f}}},
          {State::Diving, {.disp = {0.0f, 36.0f}, .size = {96.0f, 32.0f}}},
          {State::Cushioning, {.disp = {0.0f, 36.0f}, .size = {96.0f, 32.0f}}},
          {State::KickJumping,
           {.disp = {0.0f, 36.0f}, .size = {48.0f, 48.0f}}}});
}

void Scene_Play::spawnBullet(const entity_ptr &entity) {
  // TODO spawn a bullet at the given entity, going the direction the entity
  // is facing
}

void Scene_Play::update() {
  m_entities.update();
  // TODO implement pause functionality
  sMovement();
  sLifeSpan();
  sCollision();
  sState();
  sAnimationState();
  sBoundingBoxState();
  sAnimation();
  sRender();
}

void Scene_Play::sMovement() {
  auto &cTransform = m_player->get<CTransform>();
  auto &cGravity = m_player->get<CGravity>();

  assert(cTransform.exists && cGravity.exists);

  cTransform.preVelocity = cTransform.velocity;
  cTransform.velocity.y += cGravity.acc;

  cTransform.scale.x = std::copysign(
      cTransform.scale.x, cTransform.velocity.x != 0.0f ? cTransform.velocity.x
                                                        : cTransform.scale.x);

  cTransform.prevPos = cTransform.pos;
  cTransform.pos += cTransform.velocity;
}

void Scene_Play::sLifeSpan() {
  // TODO same as A2
}

void Scene_Play::sCollision() {
  auto &transform = m_player->get<CTransform>();
  auto &input = m_player->get<CInput>();
  auto &gravity = m_player->get<CGravity>();
  assert(transform.exists && input.exists && gravity.exists);

  gravity.airFrames++;
  gravity.isGrounded = false;
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
          gravity.isGrounded = true;
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

  static auto movement = [](CTransform &t, CInput &i, float v, float max) {
    t.velocity.x += i.right * v;
    t.velocity.x -= i.left * v;
    t.velocity.x = std::clamp(t.velocity.x, -max, max);
  };

  static auto friction = [](CTransform &t, float f, float cap = 1.0f) {
    auto v = t.velocity.x * f;
    t.velocity.x = v * (std::abs(v) >= cap);
  };

  static auto hasEnded = [](const CState &s, int length) {
    return s.stateTimer >= length;
  };

  for (auto &e : m_entities.getEntities()) {
    if (auto &cState = e->get<CState>(); cState.exists) {
      auto &cInput = e->get<CInput>();
      auto &cTransform = e->get<CTransform>();
      auto &cGravity = e->get<CGravity>();
      auto &cAnimation = e->get<CAnimation>();
      assert(cInput.exists && cTransform.exists && cGravity.exists);

      auto temp = cState.state;
      cInput.canJump += !cInput.up;
      cInput.canCrouch += !cInput.down;

      switch (cState.state) {
      case State::Idle:
        friction(cTransform, m_playerConfig.FRICTION);

        if (cInput.left != cInput.right) {
          cState.state = State::Running;
        } else if (cInput.up && cInput.canJump) {
          cState.state = State::Jumping;
        } else if (cTransform.velocity.y > 0) {
          cState.state = State::Falling;
        } else if (cInput.down && cInput.canCrouch) {
          cState.state = State::Crouching;
        }
        break;

      case State::FastRunning:
      case State::Running:
        friction(cTransform, m_playerConfig.FRICTION);
        movement(cTransform, cInput, m_playerConfig.SPEED + m_playerConfig.SPEED * (cState.state == State::FastRunning),
                 m_playerConfig.MAX_X_SPEED +
                     m_playerConfig.MAX_X_SPEED *
                         (cState.state == State::FastRunning));

        if (cInput.left == cInput.right &&
            std::abs(cTransform.velocity.x) < 5.0f) {
          cState.state = State::Idle;
        } else if (cInput.up && cInput.canJump) {
          cState.state = State::Jumping;
        } else if (cTransform.velocity.y > 0) {
          cState.state = State::Falling;
        } else if (cInput.left != cInput.right && cInput.down &&
                   cInput.canCrouch) {
          cState.state = State::Sliding;
        } else if (cInput.down && cInput.canCrouch) {
          cState.state = State::Crouching;
        }
        break;

      case State::Jumping:
        if (cState.stateTimer == 0) {
          cTransform.velocity.y = m_playerConfig.JUMP;
          cInput.canJump = false;
        }
        if (!cInput.up) {
          cTransform.velocity.y *= 0.5f;
        }
        movement(cTransform, cInput, m_playerConfig.SPEED,
                 m_playerConfig.MAX_X_SPEED);

        if (cTransform.velocity.y > 0) {
          cState.state = State::Falling;
        }
        break;

      case State::Falling:
        movement(cTransform, cInput, m_playerConfig.SPEED,
                 m_playerConfig.MAX_X_SPEED);

        if (cGravity.isGrounded) {
          if (cTransform.preVelocity.y > m_playerConfig.MAX_X_SPEED / 3.0f) {
            if (cInput.down) {
              cState.state = State::Cushioning;
            } else {
              cState.state = State::Landing;
            }
          } else {
            cState.state = State::Idle;
          }
        } else if (cInput.up && cInput.canJump) {
          if (cState.prevState == State::Jumping) {
            cState.state = State::KickJumping;
          } else if (cState.stateTimer < 5 &&
                     cState.prevState != State::KickJumping) {
            cState.state = State::Jumping;
          }
        }
        break;

      case State::Landing:
        friction(cTransform, m_playerConfig.FRICTION);

        if (hasEnded(cState, m_playerConfig.LANDING_DURATION)) {
          cState.state = State::Idle;
        } else if (cInput.left != cInput.right) {
          cState.state = State::Running;
        } else if (cInput.left != cInput.right && cInput.down) {
          cState.state = State::Sliding;
        } else if (cInput.up && cInput.canJump) {
          cState.state = State::Jumping;
        } else if (cInput.down && cInput.canCrouch) {
          cState.state = State::Crouching;
        }
        break;

      case State::Crouching:
        friction(cTransform, m_playerConfig.FRICTION);

        if (!cInput.down) {
          cState.state = State::Uncrouching;
        } else if (cInput.left != cInput.right) {
          cState.state = State::CrouchWalking;
        } else if (hasEnded(cState, m_playerConfig.CROUCHING_DURATION)) {
          cState.state = State::CrouchIdle;
        }
        break;

      case State::Uncrouching:
        if (cState.stateTimer == 0) {
          cInput.canCrouch = false;
        }

        friction(cTransform, m_playerConfig.FRICTION);

        if (cState.prevState == State::Cushioning &&
            cInput.left != cInput.right) {
          cState.state = State::FastRunning;
        } else if (cInput.down && cInput.canCrouch) {
          cState.state = State::Crouching;
        } else if (hasEnded(cState, m_playerConfig.CROUCHING_DURATION)) {
          cState.state = State::Idle;
        }
        break;
      case State::CrouchIdle:
        friction(cTransform, m_playerConfig.FRICTION);
        if (!cInput.down) {
          cState.state = State::Uncrouching;
        } else if (cInput.left != cInput.right) {
          cState.state = State::CrouchWalking;
        }
        break;

      case State::CrouchWalking:
        friction(cTransform, m_playerConfig.FRICTION);
        movement(cTransform, cInput, m_playerConfig.SPEED,
                 m_playerConfig.MAX_X_SPEED / 2.0f);

        if (!cInput.down) {
          cState.state = State::Uncrouching;
        } else if (std::abs(cTransform.velocity.x) < 0.1f) {
          cState.state = State::CrouchIdle;
        }
        break;

      case State::Sliding:
        if (cState.stateTimer == 0) {
          if (cInput.right) {
            cTransform.velocity.x = m_playerConfig.MAX_X_SPEED * 1.50f;
          } else if (cInput.left) {
            cTransform.velocity.x = -m_playerConfig.MAX_X_SPEED * 1.50f;
          }
        }

        movement(cTransform, cInput, 0, m_playerConfig.MAX_X_SPEED * 1.25f);

        if (!cInput.down || hasEnded(cState, m_playerConfig.SLIDING_DURATION)) {
          cState.state = State::Uncrouching;
        } else if (cInput.left != cInput.right && cInput.up && cInput.canJump) {
          cState.state = State::Diving;
        }

        break;
      case State::Diving:
        if (cState.stateTimer == 0) {
          cTransform.velocity.y = m_playerConfig.JUMP * 0.75f;
          if (cInput.right) {
            cTransform.velocity.x = m_playerConfig.MAX_X_SPEED * 2;
          } else if (cInput.left) {
            cTransform.velocity.x = -m_playerConfig.MAX_X_SPEED * 2;
          }
          cGravity.isGrounded = false;
        }

        if (cGravity.isGrounded) {
          if (cInput.down) {
            cState.state = State::Cushioning;
          } else {
            cState.state = State::Uncrouching;
          }
        }
        break;
      case State::KickJumping:
        if (cState.stateTimer == 0) {
          cTransform.velocity.y = m_playerConfig.JUMP * 0.75f;
          cInput.canJump = false;
        }

        movement(cTransform, cInput, m_playerConfig.SPEED,
                 m_playerConfig.MAX_X_SPEED);

        if (hasEnded(cState, m_playerConfig.JUMP_KICK_DURATION) &&
            cTransform.velocity.y > 0) {
          cState.state = State::Falling;
        } else if (cGravity.isGrounded) {
          cState.state = State::Uncrouching;
        }

        break;

      case State::Cushioning:
        if (cState.stateTimer == 0) {
          movement(cTransform, cInput, m_playerConfig.MAX_X_SPEED,
                   m_playerConfig.MAX_X_SPEED);
        }

        if (cInput.up && cInput.canJump) {
          cState.state = State::Diving;
        } else if (hasEnded(cState, m_playerConfig.CUSHIONING_DURATION)) {
          cState.state = State::Uncrouching;
        }
        break;
      }

      cState.stateTimer++;
      if (temp != cState.state) {
        cState.stateTimer = 0;
        cState.prevState = temp;
      }
      std::cout << stateToString(cState.state) << " "
                << stateToString(cState.prevState) << " " << cState.stateTimer
                << std::endl;
    }
  }
}

void Scene_Play::sAnimationState() {
  for (auto &e : m_entities.getEntities()) {
    if (!e->has<CStateAnimationMap>() || !e->has<CAnimation>() ||
        !e->has<CState>()) {
      continue;
    }

    const auto &cState = e->get<CState>();
    if (cState.stateTimer > 0) {
      continue;
    }

    const auto &AniMap = e->get<CStateAnimationMap>().map;
    const auto &it = AniMap.find(cState.state);
    assert(it != AniMap.end());
    e->add<CSprite>(
        e->add<CAnimation>(Assets::instance().getAnimation(it->second)));
  }
}

void Scene_Play::sBoundingBoxState() {
  for (auto &e : m_entities.getEntities()) {
    if (!e->has<CStateBoundingBoxMap>() || !e->has<CBoundingBox>() ||
        !e->has<CState>()) {
      continue;
    }

    const auto &cState = e->get<CState>();
    if (cState.stateTimer > 0) {
      continue;
    }

    const auto &BoundMap = e->get<CStateBoundingBoxMap>().map;
    const auto &it = BoundMap.find(cState.state);
    assert(it != BoundMap.end());
    e->add<CBoundingBox>(it->second.size, it->second.disp);
  }
}

void Scene_Play::sRender() {
  m_game->window().clear(sf::Color::Cyan);

  if (m_drawTextures) {
    for (auto &e : m_entities.getEntities()) {
      if (e != m_player) {
        drawTexture(e);
      }
    }
    drawTexture(m_player);
  }

  if (m_drawGrid) {
    drawGrid();
  }
  if (m_drawBoundingBox) {
    for (auto &e : m_entities.getEntities()) {
      drawBoundingBox(e);
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
    m_drawBoundingBox = !m_drawBoundingBox;
  } else if (action.name() == "toggle_texture" && action.type()) {
    m_drawTextures = !m_drawTextures;
  } else if (action.name() == "toggle_grid" && action.type()) {
    m_drawGrid = !m_drawGrid;
  }
}

void Scene_Play::drawLines(std::span<const sf::Vertex> points) {
  assert(!points.empty());
  m_game->window().draw(points.data(), points.size(), sf::PrimitiveType::Lines);
}

void Scene_Play::drawLineStrip(std::span<const sf::Vertex> points) {
  assert(!points.empty());
  m_game->window().draw(points.data(), points.size(),
                        sf::PrimitiveType::LineStrip);
}

void Scene_Play::drawBoundingBox(const entity_ptr &e) {
  if (!e->has<CBoundingBox>() || !e->has<CTransform>()) {
    return;
  }
  const auto &boundingBox = e->get<CBoundingBox>();
  auto size = boundingBox.halfSize;
  auto pos = e->get<CTransform>().pos + boundingBox.displacement;
  sf::Color color(sf::Color::Red);
  sf::Vertex points[5] = {
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y + size.y), color),
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y - size.y), color),
      sf::Vertex(sf::Vector2f(pos.x + size.x, pos.y - size.y), color),
      sf::Vertex(sf::Vector2f(pos.x + size.x, pos.y + size.y), color),
      sf::Vertex(sf::Vector2f(pos.x - size.x, pos.y + size.y), color)};

  drawLineStrip(points);
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

void Scene_Play::drawGrid() {
  std::vector<sf::Vertex> points;
  sf::Color color(sf::Color::Black);
  sf::Vector2f screen(m_game->window().getSize());

  sf::Text t(m_game->assets().getFont("simple"));
  t.setFillColor(color);
  t.setCharacterSize(16);
  for (float x = 0.0f; x < screen.x; x += m_gridSize.x) {
    points.push_back(sf::Vertex({x, 0.0f}, color));
    points.push_back(sf::Vertex({x, screen.y}, color));
  }
  for (float y = 0.0f; y < screen.y; y += m_gridSize.y) {
    points.push_back(sf::Vertex({0.0f, y}, color));
    points.push_back(sf::Vertex({screen.x, y}, color));
  }

  for (float x = 0.0f; x < screen.x; x += m_gridSize.x) {
    for (float y = 0.0f; y < screen.y; y += m_gridSize.y) {
      t.setPosition({x, y});
      t.setString(std::format("({},{})", x / m_gridSize.x,
                              (screen.y - y) / m_gridSize.y));
      m_game->window().draw(t);
    }
  }

  drawLines(points);
}
