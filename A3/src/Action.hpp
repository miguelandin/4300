#pragma once
#include <string>
#include <utility>

class Action {
  std::string m_name = "None";
  std::string m_type = "None";

public:
  Action() = default;
  Action(std::string name, std::string type)
      : m_name(std::move(name)), m_type(std::move(type)) {}

  const std::string &name() const { return m_name; }
  const std::string &type() const { return m_type; }
};
