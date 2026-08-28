#pragma once
#include "Entity.hpp"
#include <SFML/System/Vector2.hpp>

namespace Physics {
sf::Vector2f GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b) {
  // TODO: return the overlap rectangle size of the bounding boxes of entity a
  // and b
  return sf::Vector2f(0, 0);
}

sf::Vector2f GetPreviousOverlap(std::shared_ptr<Entity> a,
                                std::shared_ptr<Entity> b) {
  // TODO: return the previous overlap rectangle size of the bounding boxs of
  // entity a and b
  //   previous overlap uses the *entity's previous position*.
  return sf::Vector2f(0, 0);
}
} // namespace Physics
