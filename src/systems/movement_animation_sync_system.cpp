#include "movement_animation_sync_system.h"

#include <cstdlib>

#include "../components/animation_component.h"

namespace game {

void MovementAnimationSyncSystem::Run(ecs::World* world) {
  for (auto[sync, animation, impulse] : world->Scan<
      MovementAnimationSyncComponent,
      AnimationComponent,
      ImpulseComponent>()) {
    std::shared_ptr<Animation> next_animation = sync.current_animation;
    AnimationType next_animation_type = sync.current_animation_type;

    if (impulse.shift.isNull()) {
      if (((sync.current_animation_type == AnimationType::kDefault)
          && (animation.animation_is_ending))
          || (sync.current_animation_type != AnimationType::kDefault)) {
        auto pack = GetDefaultAnimation(sync);
        next_animation_type = pack.first;
        next_animation = pack.second;
      }
    } else {
      auto pack = GetMovAnimation(sync, impulse);
      next_animation_type = pack.first;
      next_animation = pack.second;
    }

    if (next_animation != sync.current_animation) {
      animation.SetAnimationResource(next_animation);
      sync.current_animation = next_animation;
      sync.current_animation_type = next_animation_type;
    }
  }
}

std::pair<AnimationType, std::shared_ptr<Animation>>
MovementAnimationSyncSystem::GetMovAnimation(
    const MovementAnimationSyncComponent& sync,
    const ImpulseComponent& impulse) {
  float x_shift = impulse.shift.x();
  float y_shift = impulse.shift.y();

  AnimationType deduced_type = AnimationType::_LAST;

  if (x_shift > 0) {
    deduced_type = AnimationType::kRight;
  } else if (x_shift < 0) {
    deduced_type = AnimationType::kLeft;
  } else if ((x_shift == 0) && (y_shift > 0)) {
    deduced_type = AnimationType::kDown;
  } else if ((x_shift == 0) && (y_shift < 0)) {
    deduced_type = AnimationType::kUp;
  }

  auto animation = sync.animations.find(deduced_type)->second;

  return make_pair(deduced_type, animation);
}

std::pair<AnimationType, std::shared_ptr<Animation>>
MovementAnimationSyncSystem::GetDefaultAnimation(
    const MovementAnimationSyncComponent& sync) {
  auto range_iterators = sync.animations.equal_range(AnimationType::kDefault);
  auto size = std::distance(range_iterators.first, range_iterators.second);
  std::size_t idx = std::rand() % size;
  std::advance(range_iterators.first, idx);

  auto animation = range_iterators.first->second;

  return make_pair(AnimationType::kDefault, animation);
}

}  // namespace game
