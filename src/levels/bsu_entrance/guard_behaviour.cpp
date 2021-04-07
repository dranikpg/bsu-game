#include "guard_behaviour.h"

#include "../../components/position_component.h"
#include "../../components/impulse_component.h"

game::GuardBehaviour::GuardBehaviour(ecs::Entity* player, QPoint main_pos, QPoint door_pos) :
    player_(player),
    main_position_(main_pos),
    door_position_(door_pos) {}


void game::GuardBehaviour::Process(ecs::Entity* entity) {
  auto [player_pos] = player_->Unpack<PositionComponent>();
  int distance_to_door = static_cast<int>(
      std::hypot(player_pos.position.x() - door_position_.x(),
                 player_pos.position.y() - door_position_.y()));

  auto [guard_pos, impulse] = entity->Unpack<PositionComponent, ImpulseComponent>();
  if (distance_to_door <= 180) {
    if (IsCloseToDoor(guard_pos.position)) {
      impulse.shift.setX(0);
    } else {
      impulse.shift.setX(-kGuardSpeed);
    }
  } else {
    if (guard_pos.position == main_position_) {
      impulse.shift.setX(0);
    } else {
      impulse.shift.setX(kGuardSpeed);
    }
  }
}

bool game::GuardBehaviour::IsCloseToDoor(QPoint guard_pos) const {
  return std::abs(guard_pos.x() - door_position_.x()) < 10;
}

