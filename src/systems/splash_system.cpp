#include "splash_system.h"

#include <utility>

#include "../components/splash_component.h"

namespace game {

SplashSystem::SplashSystem(context::SplashContext* splash_context,
                           context::InputContext* input_context)
    : splash_context_(splash_context), input_context_(input_context) {}


void SplashSystem::Run(ecs::World* world) {
  if (active_) {
    input_context_->BlockInput();
  }
  for (ecs::Entity& e: world->ScanEntities<SplashComponent>()) {
    auto& spc = e.GetComponent<SplashComponent>();
    active_ = true;
    splash_context_->Show(spc.icon, std::move(spc.text),
                          [this, callback = std::move(spc.callback)]() {
                            active_ = false;
                            callback();
                          });
    world->EraseEntity(&e);
  }
}

}  // namespace game
