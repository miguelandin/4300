#include "State.h"
#include "Assets.hpp"
#include "Component.hpp"
#include "Entity.hpp"
#include <cstdlib>
#include <memory>

//TODO apply friction function with parameter friction
// STAND -----------------------------------
void Stand::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("stand"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Stand::handleInput(Entity &entity, const CInput &input) {
  if (input.left != input.right) {
    return std::make_unique<Run>();
  }
  if (input.up && input.canJump) {
    return std::make_unique<Jump>();
  }
  if (input.down) {
    return std::make_unique<Crouch>();
  }

  return nullptr;
}

std::unique_ptr<State> Stand::update(Entity &entity) {
  auto &cTransform = entity.get<CTransform>();
  auto v = cTransform.velocity.x * 0.9f;
  cTransform.velocity.x = v * (std::abs(v) >= 2.0f);

  if (!entity.get<CGravity>().isGrounded) {
    return std::make_unique<Fall>();
  }
  return nullptr;
}

// RUN -------------------------------------
void Run::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("run"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Run::handleInput(Entity &entity, const CInput &input) {
  auto &cTransform = entity.get<CTransform>();

  cTransform.velocity.x += 2.0f * input.right;
  cTransform.velocity.x -= 2.0f * input.left;

  if (input.left == input.right) {
    return std::make_unique<Stand>();
  }
  if (input.up && input.canJump) {
    return std::make_unique<Jump>();
  }

  return nullptr;
}

std::unique_ptr<State> Run::update(Entity &entity) {
  auto &cTransform = entity.get<CTransform>();

  if (!entity.get<CGravity>().isGrounded) {
    return std::make_unique<Fall>();
  }

  return nullptr;
}

// JUMP ------------------------------------
void Jump::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("jump"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);

  entity.get<CTransform>().velocity.y = -26.0f;
}

std::unique_ptr<State> Jump::handleInput(Entity &entity, const CInput &input) {
  auto &cTransform = entity.get<CTransform>();

  cTransform.velocity.x += 2.0f * input.right;
  cTransform.velocity.x -= 2.0f * input.left;

  if (!input.up) {
    cTransform.velocity.y *= 0.5f;
  }
  if (input.right == input.left) { // air resistance
    cTransform.velocity.x *= 0.98f;
  }
  return nullptr;
}

std::unique_ptr<State> Jump::update(Entity &entity) {
  auto &cTransform = entity.get<CTransform>();
  if (entity.get<CGravity>().isGrounded && cTransform.velocity.y >= 0.0f) {
    return std::make_unique<Stand>();
  }
  if (cTransform.velocity.y > 0.0f) {
    return std::make_unique<Fall>();
  }
  return nullptr;
}

// FALL ------------------------------------
void Fall::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("fall"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Fall::handleInput(Entity &entity, const CInput &input) {
  auto &cTransform = entity.get<CTransform>();

  cTransform.velocity.x += 2.0f * input.right;
  cTransform.velocity.x -= 2.0f * input.left;

  if (input.right == input.left) { // air resistance
    cTransform.velocity.x *= 0.98f;
  }
  return nullptr;
}

std::unique_ptr<State> Fall::update(Entity &entity) {
  auto &cTransform = entity.get<CTransform>();

  if (entity.get<CGravity>().isGrounded && cTransform.velocity.y >= 0.0f) {
    return std::make_unique<Land>();
  }

  return nullptr;
}

// LAND ------------------------------------
void Land::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("land"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Land::handleInput(Entity &entity, const CInput &input) {
  if (input.left != input.right) {
    return std::make_unique<Run>();
  }
  if (input.up) {
    return std::make_unique<Jump>();
  }

  if (input.down) {
    return std::make_unique<Crouch>();
  }
  return nullptr;
}

std::unique_ptr<State> Land::update(Entity &entity) {
  auto &cTransform = entity.get<CTransform>();
  auto v = cTransform.velocity.x * 0.9f;
  cTransform.velocity.x = v * (std::abs(v) >= 2.0f);
  if (entity.get<CAnimation>().animation.hasEnded()) {
    return std::make_unique<Stand>();
  }
  return nullptr;
}

// Crouch ------------------------------------
void Crouch::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("crouch"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Crouch::handleInput(Entity &entity,
                                           const CInput &input) {
  if (!input.down) {
    return std::make_unique<Uncrouch>();
  }
  return nullptr;
}

std::unique_ptr<State> Crouch::update(Entity &entity) {
  if (entity.get<CAnimation>().animation.hasEnded()) {
    return std::make_unique<CrouchIdle>();
  }
  return nullptr;
}

// UnCrouch ----------------------------------
void Uncrouch::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("uncrouch"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> Uncrouch::handleInput(Entity &entity,
                                             const CInput &input) {
  if (input.down) {
    return std::make_unique<Crouch>();
  }
  return nullptr;
}

std::unique_ptr<State> Uncrouch::update(Entity &entity) {
  if (entity.get<CAnimation>().animation.hasEnded()) {
    return std::make_unique<Stand>();
  }
  return nullptr;
}

// CrouchIdle ----------------------------------
void CrouchIdle::enter(Entity &entity) {
  auto &cAnimation =
      entity.add<CAnimation>(Assets::instance().getAnimation("crouch_idle"));
  entity.add<CSprite>(cAnimation);
  entity.add<CBoundingBox>(cAnimation);
}

std::unique_ptr<State> CrouchIdle::handleInput(Entity &entity,
                                               const CInput &input) {
  if (!input.down) {
    return std::make_unique<Uncrouch>();
  }
  return nullptr;
}

std::unique_ptr<State> CrouchIdle::update(Entity &entity) { return nullptr; }
