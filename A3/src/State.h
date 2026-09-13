#pragma once
#include <memory>

class Entity;
class CInput;

class State {
public:
  virtual ~State() = default;
  virtual void enter(Entity &entity) = 0; // change sprite, bounding, etc
  virtual std::unique_ptr<State> handleInput(Entity &entity,
                                             const CInput &input) = 0;
  virtual std::unique_ptr<State> update(Entity &entity) = 0;
};

class Stand : public State {
public:
  void enter(Entity &entity) override;
  std::unique_ptr<State> handleInput(Entity &entity,
                                     const CInput &input) override;
  std::unique_ptr<State> update(Entity &entity) override;
};

class Run : public State {
public:
  void enter(Entity &entity) override;
  std::unique_ptr<State> handleInput(Entity &entity,
                                     const CInput &input) override;
  std::unique_ptr<State> update(Entity &entity) override;
};

class Jump : public State {
public:
  void enter(Entity &entity) override;
  std::unique_ptr<State> handleInput(Entity &entity,
                                     const CInput &input) override;
  std::unique_ptr<State> update(Entity &entity) override;
};

class Fall : public State {
public:
  void enter(Entity &entity) override;
  std::unique_ptr<State> handleInput(Entity &entity,
                                     const CInput &input) override;
  std::unique_ptr<State> update(Entity &entity) override;
};
