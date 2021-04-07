#include "animation_component.h"

#include <utility>

namespace game {

AnimationComponent::AnimationComponent(std::shared_ptr<resource::Animation>
    animation_resource_ptr) {
  SetAnimationResource(std::move(animation_resource_ptr));
}

void AnimationComponent::SetAnimationResource(
    std::shared_ptr<resource::Animation> animation_resource_ptr) {
  animation_resource = std::move(animation_resource_ptr);
  frame_index = 0;
  frame_time = 0;
  frame_length = animation_resource->GetFrameLength(frame_index);
  total_frames = animation_resource->GetFrameCount();
  repetitions = 0;
  animation_is_ending = false;
}

}  // namespace game
