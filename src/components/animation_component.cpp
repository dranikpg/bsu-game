#include "animation_component.h"

#include <utility>

namespace game {

AnimationComponent::AnimationComponent(std::shared_ptr<resource::Animation> animation_resource) {
  SetAnimationResource(std::move(animation_resource));
}

void AnimationComponent::SetAnimationResource(
    std::shared_ptr<resource::Animation> animation_resource) {
  animation_resource_ = std::move(animation_resource);
  frame_index = 0;
  frame_time = 0;
  frame_length = animation_resource_->GetFrameLength(frame_index);
  total_frames = animation_resource_->GetFrameCount();
  repetitions = 0;
  animation_is_ending = false;
}

}  // namespace game
