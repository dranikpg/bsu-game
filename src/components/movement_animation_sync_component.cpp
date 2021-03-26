#include "movement_animation_sync_component.h"

#include <utility>

namespace game {

MovementAnimationSyncComponent::MovementAnimationSyncComponent(
    std::multimap<constants::AnimationType,
    std::shared_ptr<resource::Animation>> animations)
     : animations(std::move(animations)),
       current_animation(animations.find(AnimationType::kDefault)->second),
       current_animation_type(AnimationType::kDefault)
       {}

}  // namespace game
