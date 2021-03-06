#include "animation_system.h"

#include "../components/sprite_component.h"
#include "../components/animation_component.h"

namespace game {

void AnimationSystem::Run(World* world) {
  for (auto[sprite, animation] : world->Scan<SpriteComponent, AnimationComponent>()) {
    ++animation.frame_time_;
    if (animation.frame_time_ > animation.frame_length_) {
      animation.frame_time_ = 1;
      ++animation.frame_index_;
      if (animation.frame_index_ >= animation.total_frames_) {
        animation.frame_index_ = 0;
        ++animation.repetitions;
      }
      animation.frame_length_ = animation.animation_resource_->GetFrameLength(animation.frame_index_);
    }

    sprite.SetGraphics(animation.animation_resource_->GetPixmap(),
                       animation.animation_resource_->GetFrame(animation.frame_index_));
  }
}

}  // namespace game
