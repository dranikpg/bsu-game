#ifndef SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
#define SRC_SYSTEMS_MOVEMENT_SYSTEM_H_

#include <QPoint>

#include "../ecs/ecs.h"

using ecs::World;
using ecs::Entity;

namespace game {

class MovementSystem : public ecs::System {
 public:
  void Run(World* world) override;
  bool IsColliding(World* world, Entity* entity, const QPoint& destination) const;
};
}  // namespace game
#endif  // SRC_SYSTEMS_MOVEMENT_SYSTEM_H_
