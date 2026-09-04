#pragma once
#include <string>
#include <utility>

class Action {
  std::string m_name = "None";
  bool m_type;

public:
  Action() = default;
  Action(std::string name, bool type) : m_name(std::move(name)), m_type(type) {}

  const std::string &name() const { return m_name; }
  bool type() const { return m_type; }
};
