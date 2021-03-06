#include "movement_system.h"

#include <array>

#include <QLineF>
#include <QRect>

#include "../components/bounds_component.h"
#include "../components/collider_component.h"
#include "../components/impulse_component.h"
#include "../components/position_component.h"

namespace game {

void MovementSystem::Run(World* world) {
  for (auto& entity : world->ScanEntities<ImpulseComponent, PositionComponent>()) {
    auto [impulse, position] = entity.Unpack<ImpulseComponent, PositionComponent>();
    QPoint dest_position(position.position + impulse.shift);
    impulse.shift = QPoint(0, 0);
    if (entity.HasComponent<ColliderComponent, BoundsComponent>() &&
        IsColliding(world, &entity, dest_position)) {
      continue;
    }
    position.position = dest_position;
  }
}

bool MovementSystem::IsColliding(World* world, Entity* entity, const QPoint& destination) const {
  auto [entity_pos, entity_bounds] = entity->Unpack<PositionComponent, BoundsComponent>();
  QRect entity_rect(0, 0, entity_bounds.bounds.x(), entity_bounds.bounds.y());
  entity_rect.moveCenter(entity_pos.position);

  QRect dest_rect(0, 0, entity_bounds.bounds.x(), entity_bounds.bounds.y());
  dest_rect.moveCenter(destination);
  std::array<QLineF, 4> movement_lines{
      QLineF(entity_rect.bottomLeft(), dest_rect.bottomLeft()),
      QLineF(entity_rect.bottomRight(), dest_rect.bottomRight()),
      QLineF(entity_rect.topLeft(), dest_rect.topLeft()),
      QLineF(entity_rect.topRight(), dest_rect.topRight())
  };

  for (auto& another_entity : world->ScanEntities<PositionComponent, ColliderComponent,
                                                  BoundsComponent>()) {
    if (entity == &another_entity) {
      continue;
    }

    auto [pos_point, bounds] = another_entity.Unpack<PositionComponent, BoundsComponent>();
    QRect colliding_rect(0, 0, bounds.bounds.x(), bounds.bounds.y());
    colliding_rect.moveCenter(pos_point.position);

    std::array<QLineF, 4> colliding_lines{
        QLineF(colliding_rect.bottomLeft(), colliding_rect.bottomRight()),
        QLineF(colliding_rect.topLeft(), colliding_rect.topRight()),
        QLineF(colliding_rect.bottomLeft(), colliding_rect.topLeft()),
        QLineF(colliding_rect.bottomRight(), colliding_rect.topRight())
    };
    QPointF i_point;
    for (const QLineF& movement_line : movement_lines) {
      for (const QLineF& colliding_line : colliding_lines) {
        if (movement_line.intersects(colliding_line, &i_point) == QLineF::BoundedIntersection) {
          return true;
        }
      }
    }
  }
  return false;
}

}  // namespace game
