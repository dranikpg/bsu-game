#include "animation_sync_system.h"

#include "../components/animation_sync_component.h"
#include "../components/animation_component.h"
#include "../components/impulse_component.h"

#include <cstdlib>

using constants::AnimationType;

namespace game {

void AnimationSyncSystem::Run(ecs::World* world) {
  DefaultHandler(world);
  ShiftHandler(world);

  for (auto[sync, animation] : world->Scan<AnimationSyncComponent, AnimationComponent>()) {
    if (sync.next_animation != sync.current_animation) {
      animation.SetAnimationResource(sync.next_animation);
      sync.current_animation = sync.next_animation;
      sync.current_animation_type = sync.next_animation_type;
    }
  }
}

void AnimationSyncSystem::ShiftHandler(ecs::World* world) {
  for (auto[sync, impulse] : world->Scan<AnimationSyncComponent, ImpulseComponent>()) {
    AnimationType deduced_type = sync.current_animation_type;
    if ((impulse.shift.x() > 0) && (impulse.shift.y() > 0)) {
      deduced_type = AnimationType::kShiftRightUpCorner;
    } else if ((impulse.shift.x() == 0) && (impulse.shift.y() > 0)) {
      deduced_type = AnimationType::kShiftUp;
    } else if ((impulse.shift.x() < 0) && (impulse.shift.y() > 0)) {
      deduced_type = AnimationType::kShiftLeftUpCorner;
    } else if ((impulse.shift.x() > 0) && (impulse.shift.y() == 0)) {
      deduced_type = AnimationType::kShiftRight;
    } else if ((impulse.shift.x() < 0) && (impulse.shift.y() == 0)) {
      deduced_type = AnimationType::kShiftLeft;
    } else if ((impulse.shift.x() > 0) && (impulse.shift.y() < 0)) {
      deduced_type = AnimationType::kShiftLeftDownCorner;
    } else if ((impulse.shift.x() == 0) && (impulse.shift.y() < 0)) {
      deduced_type = AnimationType::kShiftDown;
    } else if ((impulse.shift.x() < 0) && (impulse.shift.y() < 0)) {
      deduced_type = AnimationType::kShiftRightDownCorner;
    }

    if (sync.current_animation_type != deduced_type) {
      sync.next_animation_type = deduced_type;
      sync.next_animation = sync.animations.find(deduced_type)->second;
    }
  }
}

void AnimationSyncSystem::DefaultHandler(ecs::World* world) {
  for (auto[sync, animation] : world->Scan<AnimationSyncComponent, AnimationComponent>()) {
    if ((sync.current_animation_type == AnimationType::kDefault)
        && (animation.animation_is_ending)) {
      auto range_iterators = sync.animations.equal_range(AnimationType::kDefault);
      auto size = std::distance(range_iterators.first, range_iterators.second);
      std::size_t idx = std::rand() % size;
      std::advance(range_iterators.first, idx);
      sync.next_animation = range_iterators.first->second;
    } else if (sync.current_animation_type != AnimationType::kDefault) {
      sync.next_animation_type = AnimationType::kDefault;
      sync.next_animation = sync.animations.find(AnimationType::kDefault)->second;
    }
  }
}

}  // namespace game
