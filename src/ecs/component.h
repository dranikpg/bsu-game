#ifndef SRC_ECS_COMPONENT_H_
#define SRC_ECS_COMPONENT_H_

namespace ecs {

/**
 * Common base for all pure data structs
 */
struct Component {
  virtual ~Component() = default;
};

}  // namespace ecs

#endif  // SRC_ECS_COMPONENT_H_
