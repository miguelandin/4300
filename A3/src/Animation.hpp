#pragma once
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RenderStates.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/System/Vector2.hpp>
#include <string>

class Animation {
  std::string m_name;
  sf::Sprite m_sprite;
  int m_frameCount;
  int m_currentFrame;
  int m_speed;
  sf::Vector2i m_size;

public:
  Animation(const std::string &name, const sf::Texture &texture, int frameCount,
            int speed)
      : m_name(name), m_sprite(texture), m_frameCount(frameCount),
        m_speed(speed) {
    m_size = {m_sprite.getTextureRect().size.x / m_frameCount,
              m_sprite.getTextureRect().size.y};
    m_sprite.setTextureRect({{0, 0}, m_size});
    m_sprite.setOrigin({m_size.x / 2.0f, m_size.y / 2.0f});
  };

  void update() {
    int animFrame = (++m_currentFrame / m_speed) % m_frameCount;
    sf::IntRect rectangle{{animFrame * m_size.x, 0}, m_size};
    m_sprite.setTextureRect(rectangle);
  }

  bool hasEnded() const { return m_currentFrame / m_speed >= m_frameCount; }

  const std::string &name() const { return m_name; }

  const sf::Vector2i &size() const { return m_size; }

  const sf::Sprite &sprite() const { return m_sprite; }
};
