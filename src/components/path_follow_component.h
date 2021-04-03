#ifndef SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_
#define SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/path.h"
#include "../constants/path_follow_type.h"

using constants::PathFollowType;

namespace game {

struct PathFollowComponent : public ecs::Component {
  PathFollowComponent(std::shared_ptr<resource::Path> path_ptr,
                      PathFollowType path_type);

  std::shared_ptr<resource::Path> path;
  PathFollowType type;
};

}  // namespace game

#endif  // SRC_COMPONENTS_PATH_FOLLOW_COMPONENT_H_
