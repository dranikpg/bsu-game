#include "path_follow_system.h"

#include <QVector2D>
#include <QtMath>

#include "../components/impulse_component.h"
#include "../components/position_component.h"

using constants::PathFollowState;
using constants::PathFollowType;

namespace game {

void PathFollowSystem::Run(ecs::World* world) {
  for (auto& entity : world->ScanEntities<
      PathFollowComponent,
      PositionComponent,
      ImpulseComponent>()) {
    auto& pf_component = entity.GetComponent<PathFollowComponent>();
    HandleState(&entity);
    while (pf_component.state == PathFollowState::kResolvingWaiting ||
        pf_component.state == PathFollowState::kResolvingMoving) {
      HandleResolvingState(&entity);
      HandleState(&entity);
    }
  }
}

void PathFollowSystem::HandleState(ecs::Entity* entity) {
  auto& pf_component = entity->GetComponent<PathFollowComponent>();
  switch (pf_component.state) {
    case PathFollowState::kWaiting: {
      WaitOneTerm(&pf_component);
      break;
    }
    case PathFollowState::kMoving: {
      QPoint goal = pf_component.path.
          Point(pf_component.current_waypoint).point;
      float speed = pf_component.speed;
      MoveTowardsGoal(goal, speed, entity);
      break;
    }
    case PathFollowState::kFinished: {
      break;
    }
  }
}

void PathFollowSystem::HandleResolvingState(ecs::Entity* entity) {
  auto& pf_component = entity->GetComponent<PathFollowComponent>();
  switch (pf_component.state) {
    case PathFollowState::kResolvingMoving: {
      pf_component.state = PathFollowState::kWaiting;
      WaitOneTerm(&pf_component);
      break;
    }
    case PathFollowState::kResolvingWaiting: {
      if (SetNextGoal(&pf_component)) {
        pf_component.state = PathFollowState::kMoving;
      } else {
        pf_component.state = PathFollowState::kFinished;
      }
      break;
    }
  }
}

void PathFollowSystem::MoveTowardsGoal(QPoint goal,
                                        float speed,
                                        ecs::Entity* entity) {
  auto [position_component, impulse_component, pf_component] = entity->Unpack<
      PositionComponent, ImpulseComponent, PathFollowComponent>();
  auto position = position_component.position;
  auto& shift = impulse_component.shift;
  if (position == goal) {
    pf_component.state = PathFollowState::kResolvingMoving;
  }

  QVector2D direction(goal - position);
  direction.normalize();

  QVector2D shift_vec(direction * speed);
  QVector2D goal_to_pos(QVector2D(goal) - (QVector2D(position) + shift_vec));

  // i use (shift =) not (shift +=) cause object that use PathFollowing
  // cannot have another shift modifier
  if (abs(QVector2D::dotProduct(direction, goal_to_pos)) < 0.1f) {
    shift = goal - position;
  } else {
    shift = QPoint(ceil(shift_vec.x()),
                   ceil(shift_vec.y()));
    QPoint next_position = position + shift;
    if ((abs((goal-next_position).x()) <= 1) &&
        (abs((goal-next_position).y()) <= 1)) {
      shift = goal - position;
    }
  }
}

bool PathFollowSystem::SetNextGoal(
    PathFollowComponent* pf_component) {
  int new_waypoint_idx = 0;
  int current_waypoint_idx = pf_component->current_waypoint;
  int path_size = pf_component->total_waypoints;
  auto& state = pf_component->state;

  switch (pf_component->path_type) {
    case PathFollowType::kClosedLoop:
      new_waypoint_idx = (current_waypoint_idx + 1) % path_size;
      break;
    case PathFollowType::kLoop:
      if (current_waypoint_idx == (path_size - 1)) {
        pf_component->path.ReversePath();
        new_waypoint_idx = (path_size == 1) ? 0 : 1;
      } else {
        new_waypoint_idx = current_waypoint_idx + 1;
      }
      break;
    case PathFollowType::kOnce:
      if (current_waypoint_idx == (path_size - 1)) {
        return false;
      } else {
        new_waypoint_idx = current_waypoint_idx + 1;
      }
      break;
  }
  pf_component->SetCurrentWaypoint(new_waypoint_idx);
  return true;
}

void PathFollowSystem::WaitOneTerm(PathFollowComponent* pf_component) {
  pf_component->wait_timer++;
  if (pf_component->wait_timer > pf_component->wait_duration) {
    pf_component->state = PathFollowState::kResolvingWaiting;
  }
}

}  // namespace game