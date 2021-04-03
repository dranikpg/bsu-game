#include "path_follow_component.h"

#include <utility>

namespace game {

PathFollowComponent::PathFollowComponent(std::shared_ptr<resource::Path> path_ptr,
                                         PathFollowType type, qreal mov_speed)
                                         : path(std::move(path_ptr)),
                                           path_type(type),
                                           speed(mov_speed),
                                           state(PathFollowState::kResolvingWaiting),
                                           total_waypoints(path->Size()) {
  SetCurrentWaypoint(0);
}

void PathFollowComponent::SetCurrentWaypoint(int idx) {
  waypoint_index = idx;
  waypoint_time = 0;
  waypoint_length = path->Point(idx).pauseFrames;
}

}  // namespace game
