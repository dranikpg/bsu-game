#ifndef SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
#define SRC_SYSTEMS_MOVEMENT_SYSTEM_H_

#include <QPoint>
#include <QRect>

#include "../ecs/ecs.h"

using ecs::World;
using ecs::Entity;

namespace game {

class MovementSystem : public ecs::System {
 public:
  void Run(World* world) override;
  bool IsCollidingXY(World* world, Entity* entity,
                     const QRect& entity_rect, QPoint x_y_shift) const;
  bool IsCollidingX(World* world, Entity* entity,
                    const QRect& entity_rect, int y_shift) const;
  bool IsCollidingY(World* world, Entity* entity,
                    const QRect& entity_rect, int y_shift) const;
  bool Intersects(World* world, Entity* entity, const QRect& target) const;
};
}  // namespace game
#endif  // SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
