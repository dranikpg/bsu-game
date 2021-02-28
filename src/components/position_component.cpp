#include "position_component.h"

namespace game {

PositionComponent::PositionComponent(const QPoint& pos) : position(pos) {
}
PositionComponent::PositionComponent(int x, int y) : position(x, y) {}

}  // namespace game
