#include "path_follow_component.h"

#include <utility>

namespace game {

PathFollowComponent::PathFollowComponent(std::shared_ptr<resource::Path> path_ptr,
                                         PathFollowType path_type)
                                         : path(std::move(path_ptr)),
                                           type(path_type) {};
}  // namespace game