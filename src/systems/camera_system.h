#ifndef SRC_SYSTEMS_CAMERA_SYSTEM_H_
#define SRC_SYSTEMS_CAMERA_SYSTEM_H_

#include "../ecs/system.h"

#include "../context/window_context.h"

namespace game {

class CameraSystem : public ecs::System {
 public:
  explicit CameraSystem(context::WindowContext *window_context);
  void Run(ecs::World *world) override;
 private:
  context::WindowContext* window_context_;
  static constexpr float kLerpFactor = 0.07f;
};

}  // namespace game


#endif  // SRC_SYSTEMS_CAMERA_SYSTEM_H_
