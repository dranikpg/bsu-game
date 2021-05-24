#include "bounds_component.h"

namespace game {

BoundsComponent::BoundsComponent(const QPointF& bounds) : bounds(bounds.x(), bounds.y()) {}

BoundsComponent::BoundsComponent(float x, float y) : bounds(x, y) {}

BoundsComponent::BoundsComponent(const QSizeF& bounds) : bounds(bounds) {}

}  // namespace game
