#pragma once
#include "Animation.hpp"
#include <SFML/Audio/SoundBuffer.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <stdexcept>
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

  void addTexture(const std::string &name, const std::string &path,
                  bool smooth = false, bool repeated = false) {
    try {
      sf::Texture texture(path);
      texture.setSmooth(smooth);
      texture.setRepeated(repeated);
      m_textures.emplace(name, std::move(texture));
    } catch (const std::exception &e) {
      std::cout << "[!] Error loading TEXTURE" << std::endl;
      std::cout << "Name: " << name << ", path: " << path << std::endl;
      std::exit(1);
    }
  }

  void addSound(const std::string &name, const std::string &path) {
    try {
      m_sounds.emplace(name, path);
    } catch (const std::exception &e) {
      std::cout << "[!] Error loading SOUND" << std::endl;
      std::cout << "Name: " << name << ", path: " << path << std::endl;
      std::exit(1);
    }
  }

  void addFont(const std::string &name, const std::string &path) {
    try {
      m_fonts.emplace(std::move(name), std::move(path));
    } catch (const std::exception &e) {
      std::cout << "[!] Error loading FONT" << std::endl;
      std::cout << "Name: " << name << ", path: " << path << std::endl;
      std::exit(1);
    }
  }

  void addAnimation(std::string name, Animation animation) {
    m_animations.emplace(std::move(name), std::move(animation));
  }

  Assets(const Assets &) = delete;
  Assets &operator=(const Assets &) = delete;

public:
  Assets() = default;

  void loadFromFile(const std::string &path) {
    std::ifstream file(path);
    std::string line;

    while (std::getline(file, line)) {
      std::istringstream iss(line);
      std::string type;

      if (!(iss >> type)) {
        continue;
      };

      if (type == "Texture") {
        std::string name, filePath;
        iss >> name >> filePath;
        addTexture(name, filePath);

      } else if (type == "Animation") {
        std::string name, texture, no_repeat;
        int frames, speed;
        bool loop = true;
        iss >> name >> texture >> frames >> speed >> no_repeat;
        if (no_repeat == "no_repeat") { loop = false; }
        Animation animation(name, getTexture(texture), frames, speed, loop);
        addAnimation(std::move(name), std::move(animation));

      } else if (type == "Font") {
        std::string name, filePath;
        iss >> name >> filePath;
        addFont(name, filePath);

      } else if (type == "Sound") {
        std::string name, filePath;
        iss >> name >> filePath;
        addSound(name, filePath);
      }
    }
  }

  const sf::Texture &getTexture(const std::string &name) const {
    try {
      return m_textures.at(name);
    } catch (const std::out_of_range &e) {
      std::cout << "[!] name not found at TEXTURES: " << name << std::endl;
      std::exit(1);
    }
  }

  const sf::SoundBuffer &getSound(const std::string &name) const {
    try {
      return m_sounds.at(name);
    } catch (const std::out_of_range &e) {
      std::cout << "[!] name not found at SOUNDS: " << name << std::endl;
      std::exit(1);
    }
  }

  const sf::Font &getFont(const std::string &name) const {
    try {
      return m_fonts.at(name);
    } catch (const std::out_of_range &e) {
      std::cout << "[!] name not found at FONTS: " << name << std::endl;
      std::exit(1);
    }
  }

  const Animation &getAnimation(const std::string &name) const {
    try {
      return m_animations.at(name);
    } catch (const std::out_of_range &e) {
      std::cout << "[!] name not found at ANIMATIONS: " << name << std::endl;
      std::exit(1);
    }
  }

  const textures &getTextures() const { return m_textures; }

  const animations &getAnimations() const { return m_animations; }
};
