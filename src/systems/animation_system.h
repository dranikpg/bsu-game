#ifndef SRC_SYSTEMS_ANIMATION_SYSTEM_H_
#define SRC_SYSTEMS_ANIMATION_SYSTEM_H_

#include "../ecs/ecs.h"

using ecs::System;
using ecs::World;

namespace game {

class AnimationSystem : public System {
 public:
  AnimationSystem() = default;
  void Run(World* world) override;
};

}  // namespace game

#endif  // SRC_SYSTEMS_ANIMATION_SYSTEM_H_
