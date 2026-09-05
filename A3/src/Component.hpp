#pragma once
#include "Animation.hpp"
#include <SFML/System/Vector2.hpp>
#include <optional>
#include <string>

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

class CBoundingBox : public Component {
public:
  sf::Vector2f size;
  sf::Vector2f halfSize;

  CBoundingBox() = default;
  template <typename T>
  CBoundingBox(const sf::Vector2<T> &s)
      : size(s), halfSize(size.x / 2.0f, size.y / 2.0f) {}
};

class CAnimation : public Component {
public:
  std::optional<Animation> animation;

  CAnimation() = default;
  CAnimation(Animation animation) : animation(std::move(animation)) {}
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
