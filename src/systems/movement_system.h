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
  bool IsCollidingX(World* world, Entity* entity,
                    const QRect& entity_rect, int x_shift) const;
  bool IsCollidingY(World* world, Entity* entity,
                    const QRect& entity_rect, int y_shift) const;
  bool IsIntersects(World* world, Entity* entity, const QRect& first_case,
                    const QRect& second_case) const;
};
}  // namespace game
#endif  // SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
