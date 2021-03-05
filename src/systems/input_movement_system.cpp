#include "input_movement_system.h"

#include <set>

#include "../components/bounds_component.h"
#include "../components/impulse_component.h"
#include "../components/input_movement_component.h"

using constants::Keys;

namespace game {

InputMovementSystem::InputMovementSystem(InputContext* input) : input_(input) {}

void InputMovementSystem::Run(World* world) {
  for (auto& entity : world->ScanEntities<InputMovementComponent, ImpulseComponent>()) {
    auto [impulse] = entity.Unpack<ImpulseComponent>();
    std::set<Keys> keys = input_->GetKeys();
    impulse.shift = QPoint(0, 0);
    for (auto key : keys) {
      switch (key) {
        case Keys::kUp:
          impulse.shift += QPoint(0, -10);
          break;
        case Keys::kLeft:
          impulse.shift += QPoint(-10, 0);
          break;
        case Keys::kRight:
          impulse.shift += QPoint(10, 0);
          break;
        case Keys::kDown:
          impulse.shift += QPoint(0, 10);
          break;
      }
    }
  }
}
}  // namespace game
