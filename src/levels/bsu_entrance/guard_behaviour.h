#ifndef SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_
#define SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_

#include <QPointF>

#include "../../components/behaviour_component.h"
#include "../../ecs/entity.h"

namespace game {

class GuardBehaviour : public resource::Behaviour {
 public:
  GuardBehaviour() = default;
  GuardBehaviour(ecs::Entity*, QPointF, QPointF);
  void Process(ecs::Entity* entity) override;
  bool IsCloseToDoor(QPointF guard_pos) const;

 private:
  ecs::Entity* player_ = nullptr;
  QPointF door_position_;
  QPointF main_position_;

  const int kGuardSpeed = 8;
};
}  // namespace game
#endif  // SRC_LEVELS_BSU_ENTRANCE_GUARD_BEHAVIOUR_H_
