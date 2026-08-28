#pragma once
#include "Animation.hpp"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <fstream>
#include <map>
#include <string>

using textures = std::map<std::string, sf::Texture>;
using sounds = std::map<std::string, sf::SoundBuffer>;
using fonts = std::map<std::string, sf::Font>;
using animations = std::map<std::string, Animation>;

class Assets {
  textures m_textures;
  sounds m_sounds;
  fonts m_fonts;
  animations m_animations;

  void addTexture(std::string name, std::string path, bool smooth = false,
                  bool repeated = false) {
    auto texture = sf::Texture(std::move(path));
    texture.setSmooth(smooth);
    texture.setRepeated(repeated);
    m_textures.emplace(std::move(name), std::move(texture));
  }

  void addSound(std::string name, std::string path) {
    m_sounds.emplace(std::move(name), std::move(path));
  }

  void addFont(std::string name, std::string path) {
    m_fonts.emplace(std::move(name), std::move(path));
  }

  void addAnimation(std::string name, Animation animation) {
    m_animations.emplace(std::move(name), std::move(animation));
  }

  Assets() = default;

public:
  static Assets &Instance() {
    static Assets assets;
    return assets;
  }

  void loadFromFile(const std::string &path) {
    std::ifstream file(path);
    std::string str;

    while (file >> str) {
      if (str == "Texture") {
        std::string name, path;
        file >> name >> path;
        addTexture(std::move(name), std::move(path));
      } else if (str == "Animation") {
        std::string name, texture;
        int frames, speed;
        file >> name >> texture >> frames >> speed;
        auto animation = Animation(name, getTexture(texture), frames, speed);
        addAnimation(std::move(name), std::move(animation));
      } else if (str == "Font") {
        std::string name, path;
        file >> name >> path;
        addFont(std::move(name), std::move(path));
      } else if (str == "Sound") {
        std::string name, path;
        file >> name >> path;
        addSound(std::move(name), std::move(path));
      }
    }
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

  const textures &getTextures() const { return m_textures; }

  const animations &getAnimations() const { return m_animations; }
};
