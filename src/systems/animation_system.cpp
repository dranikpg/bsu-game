#include "animation_system.h"

#include "../components/sprite_component.h"
#include "../components/animation_component.h"

namespace game {

void AnimationSystem::Run(World* world) {
  for (auto[sprite, animation] : world->Scan<SpriteComponent, AnimationComponent>()) {
    ++animation.frame_time;
    if (animation.frame_time > animation.frame_length) {
      animation.frame_time = 1;
      ++animation.frame_index;
      if (animation.frame_index >= animation.total_frames) {
        animation.frame_index = 0;
      }
      animation.frame_length = animation.animation_resource_->GetFrameLength(animation.frame_index);
      sprite.SetGraphics(animation.animation_resource_->GetPixmap(),
                         animation.animation_resource_->GetFrame(animation.frame_index));
    }
    if ((animation.frame_index == (animation.total_frames-1))
        && (animation.frame_time == animation.frame_length)) {
      ++animation.repetitions;
      animation.animation_is_ending = true;
    } else {
      animation.animation_is_ending = false;
    }
  }
}

}  // namespace game
