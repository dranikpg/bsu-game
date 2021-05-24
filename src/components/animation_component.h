#ifndef SRC_COMPONENTS_ANIMATION_COMPONENT_H_
#define SRC_COMPONENTS_ANIMATION_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/animation.h"

namespace game {

struct AnimationComponent : public ecs::Component {
  explicit AnimationComponent(std::shared_ptr<resource::Animation> animation_resource_ptr);
  void SetAnimationResource(std::shared_ptr<resource::Animation> animation_resource_ptr);

  std::shared_ptr<resource::Animation> animation_resource;
  uint16_t frame_index;
  uint16_t frame_time;
  uint16_t frame_length;
  uint16_t total_frames;
  uint16_t repetitions;
  bool animation_is_ending;
  bool paused;
};

}  // namespace game

#endif  // SRC_COMPONENTS_ANIMATION_COMPONENT_H_
