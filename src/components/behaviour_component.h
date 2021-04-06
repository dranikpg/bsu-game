#ifndef SRC_COMPONENTS_BEHAVIOUR_COMPONENT_H_
#define SRC_COMPONENTS_BEHAVIOUR_COMPONENT_H_

#include <memory>

#include "../ecs/component.h"
#include "../resources/behaviour.h"

namespace game {

class BehaviourComponent : public ecs::Component {
 public:
  explicit BehaviourComponent(std::shared_ptr<resource::Behaviour> behaviour_resource);
  void SetBehaviourResource(std::shared_ptr<resource::Behaviour> behaviour_resource);

  std::shared_ptr<resource::Behaviour> behaviour;
};
}  // namespace game

#endif  // SRC_COMPONENTS_BEHAVIOUR_COMPONENT_H_
