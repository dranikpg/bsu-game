#include "position_component.h"

namespace game {

PositionComponent::PositionComponent(const QPoint& pos) : pos(pos) {
}
PositionComponent::PositionComponent(int x, int y) : pos(x, y) {}

}  // namespace game
