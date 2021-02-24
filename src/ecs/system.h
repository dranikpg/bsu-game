#ifndef SRC_ECS_SYSTEM_H_
#define SRC_ECS_SYSTEM_H_

namespace ecs {

class World;

class System {
 public:
  virtual ~System() = default;
  /**
   * Called on each tick
   * @param world
   */
  virtual void Run(World* world) = 0;
};

}  // namespace ecs

#endif  // SRC_ECS_SYSTEM_H_
