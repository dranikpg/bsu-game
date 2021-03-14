#ifndef SRC_COMPONENTS_IMPULSE_COMPONENT_H_
#define SRC_COMPONENTS_IMPULSE_COMPONENT_H_

#include "../ecs/component.h"

#include <QPoint>

namespace game {

struct ImpulseComponent : public ecs::Component {
  ImpulseComponent() = default;
  explicit ImpulseComponent(const QPoint& shift);
  ImpulseComponent(int x, int y);

  QPoint shift;
};
}  // namespace game

#endif  // SRC_COMPONENTS_IMPULSE_COMPONENT_H_
