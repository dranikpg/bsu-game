#ifndef SRC_SYSTEMS_SPLASH_SYSTEM_H_
#define SRC_SYSTEMS_SPLASH_SYSTEM_H_

#include "../ecs/ecs.h"
#include "../context/splash_context.h"
#include "../context/input_context.h"

namespace game {

class SplashSystem : public ecs::System {
 public:
  SplashSystem(context::SplashContext* splash_context, context::InputContext* input_context);
  void Run(ecs::World* world) override;

 private:
  bool active_ = false;
  context::SplashContext* splash_context_;
  context::InputContext* input_context_;
};

}  // namespace game

#endif  // SRC_SYSTEMS_SPLASH_SYSTEM_H_
