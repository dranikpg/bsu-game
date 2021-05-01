#ifndef SRC_COMPONENTS_MOVEMENT_ANIMATION_SYNC_COMPONENT_H_
#define SRC_COMPONENTS_MOVEMENT_ANIMATION_SYNC_COMPONENT_H_

#include <memory>
#include <map>

#include "../ecs/component.h"
#include "../resources/animation.h"
#include "../constants/animation_type.h"

using constants::AnimationType;

namespace game {

struct MovementAnimationSyncComponent : public ecs::Component {
  explicit MovementAnimationSyncComponent(std::multimap<constants::AnimationType,
                                  std::shared_ptr<resource::Animation>> animations,
                                  uint16_t pause_before_default = 0);

  std::shared_ptr<resource::Animation> current_animation;
  AnimationType current_animation_type;
  std::multimap<constants::AnimationType, std::shared_ptr<resource::Animation>> animations;
  uint16_t pause_before_default;
  uint16_t current_pause_time;
};

}  // namespace game

#endif  // SRC_COMPONENTS_MOVEMENT_ANIMATION_SYNC_COMPONENT_H_
