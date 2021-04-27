#include "movement_system.h"

#include <QLineF>

#include "../components/bounds_component.h"
#include "../components/collider_component.h"
#include "../components/impulse_component.h"
#include "../components/position_component.h"

namespace game {

void MovementSystem::Run(World* world) {
  for (auto& entity : world->ScanEntities<ImpulseComponent, PositionComponent>()) {
    auto[impulse, position] = entity.Unpack<ImpulseComponent, PositionComponent>();
    if (entity.HasComponent<ColliderComponent, BoundsComponent>()) {
      auto[entity_pos, entity_bounds] = entity.Unpack<PositionComponent, BoundsComponent>();
      QPointF destination(0, 0);
      QRectF entity_rect(0, 0, entity_bounds.bounds.x(), entity_bounds.bounds.y());
      entity_rect.moveCenter(entity_pos.position);
      if (!IsCollidingXY(world, &entity, entity_rect, impulse.shift)) {
        destination = impulse.shift;
      } else {
        if (!IsCollidingY(world, &entity, entity_rect, impulse.shift.y())) {
          destination.setY(impulse.shift.y());
        } else if (!IsCollidingX(world, &entity, entity_rect, impulse.shift.x())) {
          destination.setX(impulse.shift.x());
        }
      }
      position.position += destination;
    } else {
      position.position += impulse.shift;
    }
    impulse.shift = QPoint(0, 0);
  }
}

bool MovementSystem::Intersects(World* world, Entity* entity,
                                const QRectF& target) const {
  for (auto& another_entity : world->ScanEntities<PositionComponent, ColliderComponent,
                                                  BoundsComponent>()) {
    if (entity == &another_entity) {
      continue;
    }

    auto[pos_point, bounds] = another_entity.Unpack<PositionComponent, BoundsComponent>();
    QRectF colliding_rect(0, 0, bounds.bounds.x(), bounds.bounds.y());
    colliding_rect.moveCenter(pos_point.position);

    if (target.intersects(colliding_rect)) {
      return true;
    }
  }
  return false;
}

bool MovementSystem::IsCollidingXY(World* world,
                                  Entity* entity,
                                  const QRectF& entity_rect,
                                  QPointF x_y_shift) const {
  QRectF dest_rect = entity_rect.translated(x_y_shift.x(), x_y_shift.y());

  return Intersects(world, entity, dest_rect);
}

bool MovementSystem::IsCollidingX(World* world,
                                  Entity* entity,
                                  const QRectF& entity_rect,
                                  float x_shift) const {
  QRectF dest_rect = entity_rect.translated(x_shift, 0);

  QRectF first_collision_case(entity_rect.topLeft(), dest_rect.bottomRight());
  QRectF second_collision_case(dest_rect.topLeft(), entity_rect.bottomRight());

  return (Intersects(world, entity, first_collision_case) ||
      Intersects(world, entity, second_collision_case));
}

bool MovementSystem::IsCollidingY(World* world,
                                  Entity* entity,
                                  const QRectF& entity_rect,
                                  float y_shift) const {
  QRectF dest_rect = entity_rect.translated(0, y_shift);

  QRectF first_collision_case(dest_rect.topLeft(), entity_rect.bottomRight());
  QRectF second_collision_case(entity_rect.topLeft(), dest_rect.bottomRight());

  return (Intersects(world, entity, first_collision_case) ||
      Intersects(world, entity, second_collision_case));
}

}  // namespace game
