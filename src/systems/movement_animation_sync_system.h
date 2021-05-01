#ifndef SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_
#define SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_

#include <memory>
#include <utility>

#include "../ecs/ecs.h"
#include "../constants/animation_type.h"
#include "../resources/animation.h"
#include "../components/movement_animation_sync_component.h"
#include "../components/impulse_component.h"


using constants::AnimationType;
using resource::Animation;

class QPoint;

namespace game {

class MovementAnimationSyncSystem : public ecs::System {
 public:
  MovementAnimationSyncSystem() = default;
  void Run(ecs::World* world) override;

 private:
  std::pair<AnimationType, std::shared_ptr<Animation>>
  GetMovAnimation(const MovementAnimationSyncComponent&, const ImpulseComponent&);
  std::pair<AnimationType, std::shared_ptr<Animation>>
  GetDefaultAnimation(const MovementAnimationSyncComponent&);
  std::pair<AnimationType, std::shared_ptr<Animation>>
  GetStayPauseAnimation(const MovementAnimationSyncComponent&);
};

}  // namespace game

#endif  // SRC_SYSTEMS_MOVEMENT_ANIMATION_SYNC_SYSTEM_H_
