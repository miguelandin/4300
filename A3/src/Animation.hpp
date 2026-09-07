#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <algorithm>
#include <string>

class Animation {
public:
  std::string name = "NA";
  int frameCount = 1;
  int currentFrame = 0;
  int speed = 1;
  sf::Vector2i size;
  bool loop = false;
  const sf::Texture *texture = nullptr;

  Animation() = default;

  Animation(const std::string &name, const sf::Texture &texture, int frameCount,
            int speed, bool loop)
      : name(name), frameCount(std::max(1, frameCount)),
        speed(std::max(1, speed)), loop(loop), texture(&texture) {

    size = {static_cast<int>(texture.getSize().x) / frameCount,
            static_cast<int>(texture.getSize().y)};
  }

  bool hasEnded() const { return currentFrame / speed >= frameCount; }
};
