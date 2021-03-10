#include "impulse_component.h"

namespace game {

ImpulseComponent::ImpulseComponent(const QPoint& shift) : shift(shift) {}

ImpulseComponent::ImpulseComponent(int x, int y) : shift(x, y) {}
}  // namespace game
