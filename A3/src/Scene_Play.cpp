#include "Scene_Play.h"
#include "Action.hpp"
#include "EntityManager.hpp"
#include "Scene.hpp"
#include <SFML/System/Vector2.hpp>

Scene_Play::Scene_Play(GameEngine *gameEngine, const std::string &levelPath)
    : Scene(gameEngine), m_levelPath(levelPath) {
  init(levelPath);
}

void Scene_Play::init(const std::string &levelPath) {
  registerAction(sf::Keyboard::Scancode::P, "PAUSE");
  registerAction(sf::Keyboard::Scancode::Escape, "QUIT");
  registerAction(sf::Keyboard::Scancode::T, "TOGGLE_TEXTURE");
  registerAction(sf::Keyboard::Scancode::C, "TOGGLE_COLLISION");
  registerAction(sf::Keyboard::Scancode::G, "TOGGLE_GRID");

  // TODO register all gameplay actions
  loadLevel(levelPath);
}

sf::Vector2f Scene_Play::gridToMidPixel(float gridX, float gridY,
                                        entity_ptr entity) {

  // TODO this function take in a grid (x,y) position and an Entity
  // Return a Vec2 indicating where the CENTER position of the Entity should be
  // You must use the Entity's animations size to position it correctly
  // entity->get<CAnimation>().getSize()
  // The size of the grid width and height is stored in m_gridSize.x and
  // m_gridSize.y The bottom-left corner of the animation should align with the
  // bottom left of the grid cell Remember that SFML has(0,0) in the top left
  // corner, while grid coordinates are specified from bottom to top You can get
  // the size of the SFML window via m_game.window().getSize();
  return sf::Vector2f(0, 0);
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
  brick->add<CTransform>(sf::Vector2f(96, 480));
  brick->add<CAnimation>(m_game->assets().getAnimation("QuestionBlock"), true);
  // NOTE: Your final code should position the entity using the gridToMidPixel
  // func read from the levelFile
  // brick->add<CTransform>(gridToMidPixel(gridX,gridY,brick));
  brick->add<CBoundingBox>(
      m_game->assets().getAnimation("QuestionBlock").size());
  if (brick->get<CAnimation>().animation->name() == "QuestionBlock") {
    // this is a good way for identifying if a tile is a brick!
  }

  auto bush = m_entities.addEntity("bush");
  bush->add<CAnimation>(m_game->assets().getAnimation("Bush"), true);
  bush->add<CTransform>(sf::Vector2f(450, 450));
}

void Scene_Play::spawnPlayer() {
  if (!m_player) {
    m_player = m_entities.addEntity("Player");
  }

  m_player->add<CAnimation>(m_game->assets().getAnimation("QuestionBlock"),
                            true);
  m_player->add<CTransform>(sf::Vector2f(224, 352));
  m_player->add<CBoundingBox>(sf::Vector2f(48, 48));
  m_player->add<CState>("Stand");
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
  sAnimation();
  sRender();
}

void Scene_Play::sMovement() {
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
  // REMEMBER: SFML (0,0) position is on the TOP LEFT CORNER this means jumping
  // will have a positive y-component Also, something BELOW something else will
  // have a y value GREATER than it Also, something ABOVE something else will
  // have a y value LESS than it
  // TODO: Implement Physics::GetOverlap() function, use it inside this function
  // TODO: Implement bullet / tile collisions
  //       destroy the tile if it has a Brick animation
  // TODO: Implement player / tile collisions and resolutions update the CState
  // component of the player to store wether it is currently on the ground or in
  // the air. this will be used by the animatin system
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
      sprite.setPosition(e->get<CTransform>().pos);
      m_game->window().draw(sprite);
    }
  }
}

void Scene_Play::sDoAction(const Action &action) {}
