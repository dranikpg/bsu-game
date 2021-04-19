#include "path_follow_component.h"

#include <utility>

namespace game {

PathFollowComponent::PathFollowComponent(resource::Path mov_path,
                                         PathFollowType type, float mov_speed)
                                         : path(std::move(mov_path)),
                                           path_type(type),
                                           speed(mov_speed),
                                           state(PathFollowState::kResolvingWaiting),
                                           total_waypoints(path.Size()) {
  SetCurrentWaypoint(0);
}

void PathFollowComponent::SetCurrentWaypoint(int idx) {
  current_waypoint = idx;
  wait_timer = 0;
  wait_duration = path.Point(idx).pauseFrames;
}


}  // namespace game
