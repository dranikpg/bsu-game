#ifndef SRC_RESOURCES_BEHAVIOUR_H_
#define SRC_RESOURCES_BEHAVIOUR_H_

#include "../ecs/entity.h"

namespace resource {

class Behaviour {
 public:
  virtual ~Behaviour() = default;
  virtual void process(ecs::Entity* entity) = 0;
};

}

#endif  // SRC_RESOURCES_BEHAVIOUR_H_
