#pragma once
#include "Entity.hpp"
#include <map>
#include <memory>
#include <vector>

using EntityVec = std::vector<std::shared_ptr<Entity>>;
using EntityMap = std::map<std::string, EntityVec>;

class EntityManager {
  EntityVec m_entities;
  EntityVec m_toAdd;
  EntityMap m_entityMap;
  size_t m_totalEntities = 0;

  void removeDeadEntities(EntityVec &vec) {
    std::erase_if(vec, [](const std::shared_ptr<Entity> &entity) {
      return !entity->isAlive();
    });
  }

public:
  EntityManager() = default;

  void update() {
    for (auto e : m_toAdd) {
      m_entities.push_back(e);
      m_entityMap[e->tag()].push_back(e);
    }

    m_toAdd.clear();

    removeDeadEntities(m_entities);

    for (auto &[tag, vec] : m_entityMap) {
      removeDeadEntities(vec);
    }
  }

  std::shared_ptr<Entity> addEntity(std::string tag) {
    auto e =
        std::shared_ptr<Entity>(new Entity(std::move(tag), m_totalEntities++));
    m_toAdd.push_back(std::move(e));
    return e;
  }

  EntityVec &getEntities() { return m_entities; }

  EntityVec &getEntities(const std::string &tag) { return m_entityMap[tag]; }

  const EntityMap &getEntityMap() const { return m_entityMap; }
};
