#pragma once
#include "Animation.hpp"
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <string>
#include <utility>

class Component {
public:
  bool exists = false;
};

class CTransform : public Component {
public:
  sf::Vector2f pos = {0.0, 0.0};
  sf::Vector2f prevPos = {0.0, 0.0};
  sf::Vector2f scale = {1.0, 1.0};
  sf::Vector2f velocity = {0.0, 0.0};
  float angle;

  CTransform() = default;
  CTransform(sf::Vector2f p) : pos(p), prevPos(p) {}
  CTransform(sf::Vector2f p, sf::Vector2f sp, sf::Vector2f sc, float a)
      : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CLifespan : public Component {
public:
  int lifespan = 0;
  int frameCreated = 0;

  CLifespan() = default;
  CLifespan(int duration, int frame)
      : lifespan(duration), frameCreated(frame) {}
};

class CInput : public Component {
public:
  bool up = false;
  bool down = false;
  bool left = false;
  bool right = false;
  bool shoot = false;
  bool canShoot = false;
  bool canJump = false;

  CInput() = default;
};

class CAnimation : public Component {
public:
  Animation animation;

  CAnimation() = default;
  CAnimation(Animation animation) : animation(std::move(animation)) {}
};

class CSprite : public Component {
public:
  std::optional<sf::Sprite> sprite;
  CSprite() = default;

  CSprite(const sf::Texture &texture, int xTiles = 1, int yTiles = 1) : sprite(std::in_place, texture) {
      sf::Vector2i size(texture.getSize().x*xTiles,texture.getSize().y*yTiles);
      sprite->setTextureRect(sf::IntRect({0,0},size));
      sprite->setOrigin({size.x/2.0f,size.y/2.0f});
  }

  CSprite(const CAnimation &cAnimation)
      : sprite(std::in_place, *cAnimation.animation.texture) {
    sprite->setTextureRect(sf::IntRect({0, 0}, cAnimation.animation.size));
    sprite->setOrigin({cAnimation.animation.size.x / 2.0f, cAnimation.animation.size.y / 2.0f});
  }
};

class CBoundingBox : public Component {
public:
  sf::Vector2f size;
  sf::Vector2f halfSize;

  CBoundingBox() = default;

  CBoundingBox(const CSprite &cSprite)
      : size(cSprite.sprite->getTextureRect().size),
        halfSize(size.x / 2.0f, size.y / 2.0f) {}

  CBoundingBox(const CSprite &cSprite, float wScale, float hScale)
      : CBoundingBox(cSprite) {
    size.x *= wScale;
    size.y *= hScale;
    halfSize = {size.x / 2.0f, size.y / 2.0f};
  }
};



class CGravity : public Component {
public:
  float acc = 0;

  CGravity() = default;
  CGravity(float g) : acc(g) {}
};

class CState : public Component {
public:
  std::string state = "stand";

  CState() = default;
  CState(std::string state) : state(std::move(state)) {}
};
