#ifndef SRC_SYSTEMS_LEVEL_SYSTEM_H_
#define SRC_SYSTEMS_LEVEL_SYSTEM_H_

#include <memory>
#include <utility>

#include <QFile>

#include "../ecs/system.h"
#include "../resources/level.h"
#include "../context/level_context.h"

namespace game {

class LevelSystem : public ecs::System {
 public:
  explicit LevelSystem(context::LevelContext* context);
 private:
  using LevelRef = context::LevelContext::LevelRef;
  void Run(ecs::World* world) override;
  void LoadLevel(LevelRef level);
 private:
  LevelRef loading_level = nullptr;
  context::LevelContext* context = nullptr;
  LevelRef level = nullptr;
};

}  // namespace game

#endif  // SRC_SYSTEMS_LEVEL_SYSTEM_H_