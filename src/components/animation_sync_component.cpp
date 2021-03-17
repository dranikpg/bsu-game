#include "animation_sync_component.h"

#include <utility>

namespace game {

AnimationSyncComponent::AnimationSyncComponent(std::multimap<constants::AnimationType,
                                               std::shared_ptr<resource::Animation>> animations)
     : animations(std::move(animations)),
       current_animation(animations.find(AnimationType::kDefault)->second),
       next_animation(current_animation), current_animation_type(AnimationType::kDefault),
       next_animation_type(AnimationType::kDefault) {}

}  // namespace game
