#ifndef SRC_SYSTEMS_CAMERA_SYSTEM_H_
#define SRC_SYSTEMS_CAMERA_SYSTEM_H_

#include "../ecs/ecs.h"

namespace game {

class CameraSystem : public ecs::System {
 public:
  void Run(ecs::World* world) override;
};
}  // namespace game

#endif  // SRC_SYSTEMS_CAMERA_SYSTEM_H_
