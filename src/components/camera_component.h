#ifndef SRC_COMPONENTS_CAMERA_COMPONENT_H_
#define SRC_COMPONENTS_CAMERA_COMPONENT_H_

#include "../ecs/component.h"

namespace game {

struct CameraComponent : public ecs::Component {
  explicit CameraComponent(float scale = 1);

  float scale;
};

}  // namespace game

#endif  // SRC_COMPONENTS_CAMERA_COMPONENT_H_
