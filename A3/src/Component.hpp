#pragma once
#include "Animation.hpp"
#include "Assets.hpp"
#include <SFML/System/Vector2.hpp>

class Component {
public:
  bool exists = false;
};

class CTransfrom : public Component {
public:
  sf::Vector2f pos = {0.0, 0.0};
  sf::Vector2f prevPos = {0.0, 0.0};
  sf::Vector2f scale = {1.0, 1.0};
  sf::Vector2f velocity = {0.0, 0.0};
  float angle;

  CTransfrom() = default;
  CTransfrom(const sf::Vector2f &p) : pos(p) {}
  CTransfrom(const sf::Vector2f &p, const sf::Vector2f &sp,
             const sf::Vector2f &sc, float a)
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
