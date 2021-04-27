#include "bounds_component.h"

namespace game {

BoundsComponent::BoundsComponent(const QPointF& bounds) : bounds(bounds) {}

BoundsComponent::BoundsComponent(float x, float y) : bounds(x, y) {}

BoundsComponent::BoundsComponent(const QSizeF& bounds) :
    BoundsComponent(bounds.width(), bounds.height()) {}

}  // namespace game
