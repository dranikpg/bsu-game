#ifndef SRC_SYSTEMS_LEVEL_SYSTEM_H_
#define SRC_SYSTEMS_LEVEL_SYSTEM_H_

#include <memory>
#include <utility>

#include <QFile>

#include "../ecs/system.h"
#include "../resources/level.h"
#include "../context/level_context.h"
#include "../context/mini_game_context.h"
#include "../context/input_context.h"
#include "../context/window_context.h"

namespace game {

class LevelSystem : public ecs::System {
 public:
  LevelSystem(context::LevelContext* level_context,
              context::MiniGameContext* mini_game_context,
              context::InputContext* input_context,
              context::WindowContext* window_context);
  using LevelRef = context::LevelContext::LevelRef;

  void Run(ecs::World* world) override;
  void LoadLevel(LevelRef level);

 private:
  context::LevelContext* level_context_ = nullptr;
  context::MiniGameContext* mini_game_context_ = nullptr;
  context::InputContext* input_context_ = nullptr;
  context::WindowContext* window_context_ = nullptr;
  LevelRef loading_level_ = nullptr;
  LevelRef level_ = nullptr;
};

}  // namespace game

#endif  // SRC_SYSTEMS_LEVEL_SYSTEM_H_
