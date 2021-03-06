#ifndef SRC_COMPONENTS_ANIMATION_COMPONENT_H_
#define SRC_COMPONENTS_ANIMATION_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/animation.h"

namespace game {

class AnimationComponent : public ecs::Component {
  friend class AnimationSystem;

 public:
  explicit AnimationComponent(std::shared_ptr<resource::Animation> animation_resource);
  void SetAnimationResource(std::shared_ptr<resource::Animation> animation_resource);

 private:
  std::shared_ptr<resource::Animation> animation_resource_;
  uint16_t frame_index_;
  uint16_t frame_time_;
  uint16_t frame_length_;
  uint16_t total_frames_;
  uint16_t repetitions;
};

}  // namespace game

#endif  // SRC_COMPONENTS_ANIMATION_COMPONENT_H_
