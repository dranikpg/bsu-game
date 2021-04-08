#include "input_movement_system.h"

#include <set>

#include "../components/bounds_component.h"
#include "../components/impulse_component.h"
#include "../components/input_movement_component.h"

using constants::Keys;

namespace game {

InputMovementSystem::InputMovementSystem(InputContext* input) : input_(input) {}

void InputMovementSystem::Run(World* world) {
  for (auto[impulse, input_marker] : world->Scan<ImpulseComponent, InputMovementComponent>()) {
    std::set<Keys> keys = input_->GetKeys();
    for (auto key : keys) {
      switch (key) {
        case Keys::kUp:
          impulse.shift += QPoint(0, -kSpeed_);
          break;
        case Keys::kLeft:
          impulse.shift += QPoint(-kSpeed_, 0);
          break;
        case Keys::kRight:
          impulse.shift += QPoint(kSpeed_, 0);
          break;
        case Keys::kDown:
          impulse.shift += QPoint(0, kSpeed_);
          break;
      }
    }
  }
}
}  // namespace game
