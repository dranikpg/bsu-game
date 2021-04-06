#ifndef SRC_SYSTEMS_BEHAVIOUR_SYSTEM_H_
#define SRC_SYSTEMS_BEHAVIOUR_SYSTEM_H_

#include "../ecs/system.h"
#include "../ecs/world.h"

namespace game {

class BehaviourSystem : public ecs::System {
 public:
  BehaviourSystem() = default;
  void Run(ecs::World* world) override;
};
}  // namespace game
#endif  // SRC_SYSTEMS_BEHAVIOUR_SYSTEM_H_
