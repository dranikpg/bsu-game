#include "movement_animation_sync_component.h"

#include <utility>

namespace game {

MovementAnimationSyncComponent::MovementAnimationSyncComponent(
    std::multimap<constants::AnimationType,
    std::shared_ptr<resource::Animation>> animations,
    uint16_t pause_before_default)
     : animations(std::move(animations)),
       current_animation(animations.find(AnimationType::kDefault)->second),
       current_animation_type(AnimationType::kDefault),
       pause_before_default(pause_before_default),
       current_pause_time(0)
       {};

}  // namespace game
