#ifndef SRC_ECS_WORLD_H_
#define SRC_ECS_WORLD_H_

#include <memory>
#include <functional>
#include <utility>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <type_traits>

#include "system.h"
#include "entity.h"
#include "entity_iterators.h"

namespace ecs {

/**
 * Ecs world which holds all entities and components
 */
class World {
 public:
  ~World();
  /**
   * Init world
   * @param systems
   */
  void Init(std::vector<std::unique_ptr<System>>&& systems);
  /**
   * Create single entity
   * @return
   */
  Entity& CreateEntity();
  /**
   * Erase single entity
   * @param const_reference
   */
  void EraseEntity(const Entity& const_reference);
  /**
   * Iterate over entities with specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  EntityIterator<Ts...> ScanEntities() {
    return EntityIterator<Ts...>(entities_.begin(), entities_.end());
  }
  /**
   * Iterate over specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  ComponentIterator<Ts...> Scan() {
    return ComponentIterator<Ts...>(
        EntityIterator<Ts...>(entities_.begin(), entities_.end()),
        EntityIterator<Ts...>(entities_.end(), entities_.end()));
  }
  /**
   * Run simulations
   */
  void Run();

 private:
  /**
   * Remove and Create pending entities
   */
  void SyncEntities();

  std::unordered_set<std::shared_ptr<Entity>> entities_;
  std::unordered_set<std::shared_ptr<Entity>> entities_created_;
  std::vector<std::shared_ptr<Entity>> entities_deleted_;
  std::vector<std::unique_ptr<System>> systems_;
};

}  // namespace ecs

#endif  // SRC_ECS_WORLD_H_
