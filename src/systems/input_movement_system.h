#ifndef SRC_SYSTEMS_INPUT_MOVEMENT_SYSTEM_H_
#define SRC_SYSTEMS_INPUT_MOVEMENT_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../context/input_context.h"

using ecs::World;
using context::InputContext;

namespace game {

class InputMovementSystem : public ecs::System {
 public:
  explicit InputMovementSystem(InputContext* input);
  void Run(World* world) override;

 private:
  InputContext* input_;
  const int kSpeed_ = 10;
};
}  // namespace game
#endif  // SRC_SYSTEMS_INPUT_MOVEMENT_SYSTEM_H_
