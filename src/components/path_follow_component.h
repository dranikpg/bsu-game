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
  PathFollowComponent(std::shared_ptr<resource::Path> path_ptr,
                      PathFollowType type,
                      qreal mov_speed);
  void SetCurrentWaypoint(int idx);

  std::shared_ptr<resource::Path> path;
  PathFollowType path_type;
  PathFollowState state;
  // when state kWaiting stores current WayPoint, when state kMoving stores next WayPoint
  int waypoint_index;
  int waypoint_time;
  int waypoint_length;
  int total_waypoints;
  qreal speed;
};

}  // namespace game

#endif  // SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_
