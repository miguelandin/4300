#pragma once
#include "Entity.hpp"
#include <SFML/System/Vector2.hpp>
#include <cassert>
#include <cstdlib>
#include <memory>

namespace Physics {
inline sf::Vector2f calculateOverlap(sf::Vector2f posA, sf::Vector2f posB,
                                     sf::Vector2f halfSizeA,
                                     sf::Vector2f halfSizeB) {
  sf::Vector2f delta(std::abs(posA.x - posB.x), std::abs(posA.y - posB.y));
  float ox = (halfSizeA.x + halfSizeB.x) - delta.x;
  float oy = (halfSizeA.y + halfSizeB.y) - delta.y;
  return sf::Vector2f(ox, oy);
}

inline sf::Vector2f getOverlap(const std::shared_ptr<Entity> &a,
                               const std::shared_ptr<Entity> &b) {
  auto boundingA = a->get<CBoundingBox>();
  auto boundingB = b->get<CBoundingBox>();
  auto transformA = a->get<CTransform>();
  auto transformB = b->get<CTransform>();
  assert(boundingA.exists && boundingB.exists && transformA.exists &&
         transformB.exists);

  return calculateOverlap(transformA.pos, transformB.pos, boundingA.halfSize,
                          boundingB.halfSize);
}

inline sf::Vector2f GetPreviousOverlap(const std::shared_ptr<Entity> &a,
                                       const std::shared_ptr<Entity> &b) {
  auto boundingA = a->get<CBoundingBox>();
  auto boundingB = b->get<CBoundingBox>();
  auto transformA = a->get<CTransform>();
  auto transformB = b->get<CTransform>();
  assert(boundingA.exists && boundingB.exists && transformA.exists &&
         transformB.exists);

  return calculateOverlap(transformA.prevPos, transformB.prevPos,
                          boundingA.halfSize, boundingB.halfSize);
}
} // namespace Physics
