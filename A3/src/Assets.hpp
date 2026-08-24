#pragma once
#include "Animation.hpp"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <map>
#include <string>

class Assets {
  std::map<std::string, sf::Texture> m_textures;
  std::map<std::string, sf::SoundBuffer> m_sounds;
  std::map<std::string, sf::Font> m_fonts;
  std::map<std::string, Animation> m_animations;

public:
  void addTexture(const std::string &name, const std::string &path) {
    m_textures.emplace(name, path);
  }

  void addSound(const std::string &name, const std::string &path) {
    m_sounds.emplace(name, path);
  }

  void addFont(const std::string &name, const std::string &path) {
    m_fonts.emplace(name, path);
  }

  void addAnimation(const std::string &name, const Animation &animation) {
    m_animations.emplace(name, animation);
  }

  const sf::Texture &getTexture(const std::string &name) const {
    return m_textures.at(name);
  }

  const sf::SoundBuffer &getSound(const std::string &name) const {
    return m_sounds.at(name);
  }

  const sf::Font &getFont(const std::string &name) const {
    return m_fonts.at(name);
  }

  const Animation &getAnimation(const std::string &name) const {
    return m_animations.at(name);
  }
};
