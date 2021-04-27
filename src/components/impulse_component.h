#ifndef SRC_COMPONENTS_IMPULSE_COMPONENT_H_
#define SRC_COMPONENTS_IMPULSE_COMPONENT_H_

#include "../ecs/component.h"

#include <QPointF>

namespace game {

struct ImpulseComponent : public ecs::Component {
  ImpulseComponent() = default;
  explicit ImpulseComponent(const QPointF& shift);
  ImpulseComponent(float x, float y);

  QPointF shift;
};
}  // namespace game

#endif  // SRC_COMPONENTS_IMPULSE_COMPONENT_H_
