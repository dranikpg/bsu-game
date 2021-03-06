#include "animation_component.h"

#include <utility>

namespace game {

AnimationComponent::AnimationComponent(std::shared_ptr<resource::Animation> animation_resource) {
  SetAnimationResource(std::move(animation_resource));
}

void AnimationComponent::SetAnimationResource(std::shared_ptr<resource::Animation> animation_resource) {
  animation_resource_ = std::move(animation_resource);
  frame_index_ = 0;
  frame_time_ = 0;
  frame_length_ = animation_resource_->GetFrameLength(frame_index_);
  total_frames_ = animation_resource_->GetFrameCount();
  repetitions = 0;
}

}  // namespace game