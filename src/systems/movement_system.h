#ifndef SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
#define SRC_SYSTEMS_MOVEMENT_SYSTEM_H_

#include <QPointF>
#include <QRectF>

#include "../ecs/ecs.h"

using ecs::World;
using ecs::Entity;

namespace game {

class MovementSystem : public ecs::System {
 public:
  void Run(World* world) override;
  bool IsCollidingXY(World* world, Entity* entity,
                     const QRectF& entity_rect, QPointF x_y_shift) const;
  bool IsCollidingX(World* world, Entity* entity,
                    const QRectF& entity_rect, float y_shift) const;
  bool IsCollidingY(World* world, Entity* entity,
                    const QRectF& entity_rect, float y_shift) const;
  bool Intersects(World* world, Entity* entity, const QRectF& target) const;
};
}  // namespace game
#endif  // SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
