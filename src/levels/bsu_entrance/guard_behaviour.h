#ifndef SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_
#define SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_

#include <QPoint>
#include "../../components/behaviour_component.h"
#include "../../ecs/entity.h"

namespace game {

class GuardBehaviour : public resource::Behaviour {
 public:
  GuardBehaviour() = default;
  GuardBehaviour(ecs::Entity*, QPoint, QPoint);
  void Process(ecs::Entity* entity) override;
  bool IsCloseToDoor(QPoint guard_pos) const;

 private:
  ecs::Entity* player_ = nullptr;
  QPoint door_position_;
  QPoint main_position_;

  const int kGuardSpeed = 8;
};
}  // namespace game
#endif  // SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_
