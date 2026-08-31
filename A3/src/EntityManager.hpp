#pragma once
#include "Entity.hpp"
#include <map>
#include <memory>
#include <vector>

using entity_ptr = std::shared_ptr<Entity>;
using entity_vec = std::vector<entity_ptr>;
using entity_map = std::map<std::string, entity_vec>;

class EntityManager {
  entity_vec m_entities;
  entity_vec m_toAdd;
  entity_map m_entityMap;
  size_t m_totalEntities = 0;

  void removeDeadEntities(entity_vec &vec) {
    std::erase_if(vec,
                  [](const entity_ptr &entity) { return !entity->isAlive(); });
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

  entity_ptr addEntity(std::string tag) {
    auto e = entity_ptr(new Entity(std::move(tag), m_totalEntities++));
    m_toAdd.push_back(std::move(e));
    return e;
  }

  entity_vec &getEntities() { return m_entities; }

  entity_vec &getEntities(const std::string &tag) { return m_entityMap[tag]; }

  const entity_map &getEntityMap() const { return m_entityMap; }
};
