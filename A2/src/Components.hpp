#pragma once

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include "Vec2.hpp"

class Component
{
public:
  bool exists = false;
};

class CTransform : public Component
{
public:
  Vec2f pos = {0.0, 0.0};
  Vec2f velocity = {0.0, 0.0};
  float angle = 0;

  CTransform() = default;
  CTransform(const Vec2f &p, const Vec2f &v, float a) : pos(p), velocity(v), angle(a) {}
};

class CShape : public Component
{
public:
  sf::CircleShape circle;
  CShape() = default;
  CShape(float radius, size_t points, const sf::Color &fill, const sf::Color &outline, float thickness)
      : circle(radius, points)
  {
    circle.setFillColor(fill);
    circle.setOutlineColor(outline);
    circle.setOutlineThickness(thickness);
    circle.setOrigin({radius, radius});
  }
};

class CCollision : public Component
{
public:
  float radius = 0;
  uint8_t type = 0b00000000;
  uint8_t mask = 0b00000000;

  CCollision() = default;
  CCollision(float r, uint8_t t, uint8_t m) : radius(r), type(t), mask(m) {}
};

class CScore : public Component
{
public:
  int score = 0;
  CScore() = default;
  CScore(int s) : score(s) {}
};

class CLifespan : public Component
{
public:
  int lifespan = 0;  // no cambia
  int remaining = 0; // va bajando
  CLifespan() = default;
  CLifespan(int totalLifespan) : lifespan(totalLifespan), remaining(totalLifespan) {}
};

class CInput : public Component
{
public:
  bool w = false;
  bool a = false;
  bool s = false;
  bool d = false;
  bool up = false;
  bool left = false;
  bool right = false;
  bool down = false;
  bool shoot = false;
  bool special = false;

  CInput() = default;
};

class CBounciness : public Component
{
public:
  int remaining = 0;
  bool infinite = false;

  CBounciness() = default;
};

class CWeapon : public Component
{
public:
  int fireRate = 15;
  int lastFired = 0;

  CWeapon() = default;
  CWeapon(int r) : fireRate(r) {}
};

class CExplosion : public Component
{
public:
  uint8_t type = 0;
  int duration = 0;
  CExplosion() = default;
  CExplosion(uint8_t t, int d) : type(t), duration(d) {}
};

class CSpecial : public Component
{
public:
  int cooldown = 30;
  int lastFired = 0;
  CSpecial() = default;
  CSpecial(int c) : cooldown(c) {}
};

class CInterface : public Component
{
public:
  sf::RectangleShape barBack;
  sf::RectangleShape barFront;
  CInterface() = default;
  CInterface(float width, float height, const sf::Color &fill, const sf::Color &outline, float thickness, float radius)
      : barBack({width, height}), barFront({0, height})
  {
    float x = width / 2.0f;
    float y = height + radius + 10.0f;
    barBack.setFillColor({255, 255, 255});
    barBack.setOutlineColor({255, 255, 255});
    barBack.setOutlineThickness(thickness);
    barBack.setOrigin({x, y});
    barFront.setFillColor(fill);
    barFront.setOutlineColor(outline);
    barFront.setOutlineThickness(thickness);
    barFront.setOrigin({x, y});
  }
};
