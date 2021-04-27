#include "position_component.h"

namespace game {

PositionComponent::PositionComponent(const QPointF& pos) : position(pos) {
}
PositionComponent::PositionComponent(float x, float y) : position(x, y) {}

}  // namespace game
