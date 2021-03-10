#include "bounds_component.h"

namespace game {

BoundsComponent::BoundsComponent(const QPoint& bounds) : bounds(bounds) {}

BoundsComponent::BoundsComponent(int x, int y) : bounds(x, y) {}
}  // namespace game
