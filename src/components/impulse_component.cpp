#include "impulse_component.h"

namespace game {

ImpulseComponent::ImpulseComponent(const QPointF& shift) : shift(shift) {}

ImpulseComponent::ImpulseComponent(float x, float y) : shift(x, y) {}
}  // namespace game
