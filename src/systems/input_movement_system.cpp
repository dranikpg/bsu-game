#include "input_movement_system.h"

#include <set>
#include <cmath>

#include "../components/bounds_component.h"
#include "../components/impulse_component.h"
#include "../components/input_movement_component.h"
#include <QDebug>

using constants::Keys;

namespace game {

InputMovementSystem::InputMovementSystem(InputContext* input) : input_(input) {}

void InputMovementSystem::Run(World* world) {
  for (auto[impulse, input_marker] : world->Scan<ImpulseComponent, InputMovementComponent>()) {
    std::set<Keys> keys = input_->GetKeys();
    impulse.shift = QPointF(0, 0);
    for (auto key : keys) {
      switch (key) {
        case Keys::kUp:
          impulse.shift += QPoint(0, -1);
          break;
        case Keys::kLeft:
          impulse.shift += QPoint(-1, 0);
          break;
        case Keys::kRight:
          impulse.shift += QPoint(1, 0);
          break;
        case Keys::kDown:
          impulse.shift += QPoint(0, 1);
          break;
        default:
          break;
      }
    }
    if (impulse.shift.isNull()) {
      continue;
    }
    float length = std::hypotf(impulse.shift.x(), impulse.shift.y());
    impulse.shift.rx() *= kSpeed_ / length;
    impulse.shift.ry() *= kSpeed_ / length;
  }
}
}  // namespace game
