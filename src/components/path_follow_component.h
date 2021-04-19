#ifndef SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_
#define SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/path.h"
#include "../constants/path_follow.h"

using constants::PathFollowType;
using constants::PathFollowState;

namespace game {

struct PathFollowComponent : public ecs::Component {
  PathFollowComponent() = default;
  PathFollowComponent(resource::Path mov_path,
                      PathFollowType type,
                      float mov_speed);
  void SetCurrentWaypoint(int idx);

  resource::Path path;
  PathFollowType path_type;
  PathFollowState state;
  // when state kWaiting stores current WayPoint, when state kMoving stores next WayPoint
  int current_waypoint;
  int wait_timer;
  int wait_duration;
  int total_waypoints;
  float speed;
};

}  // namespace game

#endif  // SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_
