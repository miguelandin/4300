#include "Action.hpp"
#include "EntityManager.hpp"
#include "GameEngine.hpp"

using actionMap = std::map<int, std::string>;

class Scene {
protected:
  GameEngine *m_game = nullptr;
  EntityManager m_entities;
  actionMap m_actionMap;
  size_t m_frame = 0;
  bool m_paused = false;

public:
  Scene() = default;

  ~Scene() = default;

  explicit Scene(GameEngine *gameEngine) : m_game(gameEngine) {}

  virtual void update() = 0;

  virtual void sDoAction(const Action &action) = 0;

  virtual void sRender() = 0;

  void simulate(const size_t frames) {
    for (size_t i = 0; i < frames; i++) {
      update();
    }
  }

  void doAction(const Action &action) { sDoAction(action); }

  void registerAction(int code, const std::string &name) {
    m_actionMap[code] = name;
  }

  void setPaused(bool paused) { m_paused = paused; }

  size_t frame() const { return m_frame; }
};
