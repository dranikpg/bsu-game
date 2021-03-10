#include "movement_system.h"

#include <array>
#include <QLineF>

#include "../components/bounds_component.h"
#include "../components/collider_component.h"
#include "../components/impulse_component.h"
#include "../components/position_component.h"

namespace game {

void MovementSystem::Run(World* world) {
  for (auto& entity : world->ScanEntities<ImpulseComponent, PositionComponent>()) {
    auto [impulse, position] = entity.Unpack<ImpulseComponent, PositionComponent>();
    if (entity.HasComponent<ColliderComponent, BoundsComponent>()) {
      auto [entity_pos, entity_bounds] = entity.Unpack<PositionComponent, BoundsComponent>();
      QPoint destination(0, 0);

      QRect entity_rect(0, 0, entity_bounds.bounds.x(), entity_bounds.bounds.y());
      entity_rect.moveCenter(entity_pos.position);
      if (!IsCollidingY(world, &entity, entity_rect, impulse.shift.y())) {
        destination.setY(impulse.shift.y());
      }
      if (!IsCollidingX(world, &entity, entity_rect, impulse.shift.x())) {
        destination.setX(impulse.shift.x());
      }
      position.position += destination;
    }
    impulse.shift = QPoint(0, 0);
  }
}

bool MovementSystem::IsIntersects(World* world, Entity* entity,
                                  const QRect& first_case,
                                  const QRect& second_case) const {
  for (auto& another_entity : world->ScanEntities<PositionComponent, ColliderComponent,
                                                  BoundsComponent>()) {
    if (entity == &another_entity) {
      continue;
    }

    auto [pos_point, bounds] = another_entity.Unpack<PositionComponent, BoundsComponent>();
    QRect colliding_rect(0, 0, bounds.bounds.x(), bounds.bounds.y());
    colliding_rect.moveCenter(pos_point.position);

    if (first_case.intersects(colliding_rect) ||
        second_case.intersects(colliding_rect)) {
      return true;
    }
  }
  return false;
}

bool MovementSystem::IsCollidingX(World* world,
                                  Entity* entity,
                                  const QRect& entity_rect,
                                  int x_shift) const {
  QRect dest_rect = entity_rect;
  dest_rect.moveCenter(QPoint(entity_rect.center().x() + x_shift,
                                       entity_rect.center().y()));

  QRect first_collision_case(entity_rect.topLeft(), dest_rect.bottomRight());
  QRect second_collision_case(dest_rect.topLeft(), entity_rect.bottomRight());

  return IsIntersects(world, entity, first_collision_case, second_collision_case);
}

bool MovementSystem::IsCollidingY(World* world,
                                  Entity* entity,
                                  const QRect& entity_rect,
                                  int y_shift) const {
  QRect dest_rect = entity_rect;
  dest_rect.moveCenter(QPoint(entity_rect.center().x(),
                              entity_rect.center().y() + y_shift));

  QRect first_collision_case(dest_rect.topLeft(), entity_rect.bottomRight());
  QRect second_collision_case(entity_rect.topLeft(), dest_rect.bottomRight());

  return IsIntersects(world, entity, first_collision_case, second_collision_case);
}

}  // namespace game
