#ifndef SRC_COMPONENTS_POSITION_COMPONENT_H_
#define SRC_COMPONENTS_POSITION_COMPONENT_H_

#include <QPointF>

#include "../ecs/component.h"

namespace game {

struct PositionComponent : public ecs::Component {
  PositionComponent() = default;
  explicit PositionComponent(const QPointF& pos);

  PositionComponent(float x, float y);

  QPointF position;
};

}  // namespace game

#endif  // SRC_COMPONENTS_POSITION_COMPONENT_H_
