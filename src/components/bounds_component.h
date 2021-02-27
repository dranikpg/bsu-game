#ifndef SRC_COMPONENTS_BOUNDS_COMPONENT_H_
#define SRC_COMPONENTS_BOUNDS_COMPONENT_H_

#include "../ecs/component.h"

#include <QPoint>

namespace game {

struct BoundsComponent : ecs::Component {
  BoundsComponent() = default;
  explicit BoundsComponent(const QPoint& bounds);
  BoundsComponent(int x, int y);

  QPoint bounds;
};
}
#endif  // SRC_COMPONENTS_BOUNDS_COMPONENT_H_
