#pragma once
#include "Component.hpp"
#include "Entity.hpp"
#include "EntityManager.hpp"
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

namespace detail {
inline sf::Vector2f overlapAt(const entity_ptr &a, const entity_ptr &b,
                              sf::Vector2f CTransform::*posField) {
  const auto &bA = a->get<CBoundingBox>();
  const auto &bB = b->get<CBoundingBox>();
  const auto &tA = a->get<CTransform>();
  const auto &tB = b->get<CTransform>();

  assert(bA.exists && bB.exists && tA.exists && tB.exists);

  return calculateOverlap(tA.*posField + bA.displacement,
                          tB.*posField + bB.displacement, bA.halfSize,
                          bB.halfSize);
}
}; // namespace detail

inline sf::Vector2f getOverlap(const entity_ptr &a, const entity_ptr &b) {
  return detail::overlapAt(a, b, &CTransform::pos);
}

inline sf::Vector2f getPreviousOverlap(const std::shared_ptr<Entity> &a,
                                       const std::shared_ptr<Entity> &b) {
  return detail::overlapAt(a, b, &CTransform::prevPos);
}
} // namespace Physics
