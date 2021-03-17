#ifndef SRC_SYSTEMS_ANIMATION_SYNC_SYSTEM_H_
#define SRC_SYSTEMS_ANIMATION_SYNC_SYSTEM_H_

#include "../ecs/ecs.h"

namespace game {

class AnimationSyncSystem : public ecs::System {
 public:
  AnimationSyncSystem() = default;
  void Run(ecs::World* world) override;

 private:
  void ShiftHandler(ecs::World* world);
  void DefaultHandler(ecs::World* world);
};

}  // namespace game

#endif  // SRC_SYSTEMS_ANIMATION_SYNC_SYSTEM_H_
