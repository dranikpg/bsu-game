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
#include "entity_iterator.h"
#include "component_iterator.h"

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
  void EraseEntity(Entity* ptr);
  /**
   * Iterate over entities with specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  EntityIterator<Ts...> ScanEntities();
  /**
   * Iterate over specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  ComponentIterator<Ts...> Scan();;
  /**
   * Run simulations
   */
  void Run();

 private:
  /**
   * Remove and Create pending entities
   */
  void SyncEntities();

 private:
  std::unordered_set<std::shared_ptr<Entity>> entities_;
  std::unordered_set<std::shared_ptr<Entity>> entities_created_;
  std::vector<std::shared_ptr<Entity>> entities_deleted_;
  std::vector<std::unique_ptr<System>> systems_;
};

template<typename... Ts>
EntityIterator<Ts...> World::ScanEntities() {
  return EntityIterator<Ts...>(entities_.begin(), entities_.end());
}

template<typename... Ts>
ComponentIterator<Ts...> World::Scan() {
  return ComponentIterator<Ts...>(
      EntityIterator<Ts...>(entities_.begin(), entities_.end()),
      EntityIterator<Ts...>(entities_.end(), entities_.end()));
}

}  // namespace ecs

#endif  // SRC_ECS_WORLD_H_
