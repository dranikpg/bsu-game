#include "movement_animation_sync_system.h"

#include "../components/movement_animation_sync_component.h"
#include "../components/animation_component.h"
#include "../components/impulse_component.h"

#include <cstdlib>

using constants::AnimationType;

namespace game {

void MovementAnimationSyncSystem::Run(ecs::World* world) {
  DefaultHandler(world);
  ShiftHandler(world);

  for (auto[sync, animation] : world->Scan<MovementAnimationSyncComponent, AnimationComponent>()) {
    if (sync.next_animation != sync.current_animation) {
      animation.SetAnimationResource(sync.next_animation);
      sync.current_animation = sync.next_animation;
      sync.current_animation_type = sync.next_animation_type;
    }
  }
}

void MovementAnimationSyncSystem::ShiftHandler(ecs::World* world) {
  for (auto[sync, impulse] : world->Scan<MovementAnimationSyncComponent, ImpulseComponent>()) {
    AnimationType deduced_type = sync.current_animation_type;
    int xshift = impulse.shift.x();
    int yshift = impulse.shift.y();

    if (xshift > 0) {
      deduced_type = AnimationType::kRight;
    } else if (xshift < 0) {
      deduced_type = AnimationType::kLeft;
    } else if ((xshift == 0) && (yshift > 0)) {
      deduced_type = AnimationType::kDown;
    } else if ((xshift == 0) && (yshift < 0)) {
      deduced_type = AnimationType::kUp;
    }

    if (sync.current_animation_type != deduced_type) {
      sync.next_animation_type = deduced_type;
      sync.next_animation = sync.animations.find(deduced_type)->second;
    }
  }
}

void MovementAnimationSyncSystem::DefaultHandler(ecs::World* world) {
  for (auto[sync, animation] : world->Scan<MovementAnimationSyncComponent, AnimationComponent>()) {
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
