#ifndef SRC_COMPONENTS_BOUNDS_COMPONENT_H_
#define SRC_COMPONENTS_BOUNDS_COMPONENT_H_

#include "../ecs/component.h"

#include <QPointF>
#include <QSizeF>

namespace game {

struct BoundsComponent : ecs::Component {
  BoundsComponent() = default;
  explicit BoundsComponent(const QPointF& bounds);
  explicit BoundsComponent(const QSizeF& bounds);
  BoundsComponent(float x, float y);

  QPointF bounds;
};

}  // namespace game

#endif  // SRC_COMPONENTS_BOUNDS_COMPONENT_H_
