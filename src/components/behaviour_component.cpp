#include "behaviour_component.h"

#include <utility>

namespace game {

BehaviourComponent::BehaviourComponent(std::shared_ptr<resource::Behaviour> behaviour_resource) {
  behaviour = std::move(behaviour_resource);
}

void BehaviourComponent::SetBehaviourResource(
    std::shared_ptr<resource::Behaviour> behaviour_resource) {
  behaviour = std::move(behaviour_resource);
}
}  // namespace game
