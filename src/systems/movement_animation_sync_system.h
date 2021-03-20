#ifndef SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_
#define SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_

#include "../ecs/ecs.h"

namespace game {

class MovementAnimationSyncSystem : public ecs::System {
 public:
  MovementAnimationSyncSystem() = default;
  void Run(ecs::World* world) override;

 private:
  void ShiftHandler(ecs::World* world);
  void DefaultHandler(ecs::World* world);
};

}  // namespace game

#endif  // SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_
