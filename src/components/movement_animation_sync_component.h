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
                                  std::shared_ptr<resource::Animation>> animations);

  std::shared_ptr<resource::Animation> current_animation;
  AnimationType current_animation_type;
  std::shared_ptr<resource::Animation> next_animation;
  AnimationType next_animation_type;
  std::multimap<constants::AnimationType, std::shared_ptr<resource::Animation>> animations;
};

}  // namespace game

#endif  // SRC_COMPONENTS_MOVEMENT_ANIMATION_SYNC_COMPONENT_H_
