#ifndef SRC_SYSTEMS_PATH_FOLLOW_SYSTEM_H_
#define SRC_SYSTEMS_PATH_FOLLOW_SYSTEM_H_

#include <QPoint>

#include "../ecs/ecs.h"
#include "../components/path_follow_component.h"
#include "../resources/path.h"

namespace game {

class PathFollowSystem : public ecs::System {
 public:
  PathFollowSystem() = default;
  void Run(ecs::World* world) override;

 private:
  void MoveTowardsGoal(QPoint goal,
                        float speed,
                        ecs::Entity* entity);
  bool SetNextGoal(PathFollowComponent* pf_component);
  void WaitOneTerm(PathFollowComponent* pf_component);
  void HandleState(ecs::Entity* entity);
  void HandleResolvingState(ecs::Entity* entity);
};

}  // namespace game

#endif  // SRC_SYSTEMS_PATH_FOLLOW_SYSTEM_H_
