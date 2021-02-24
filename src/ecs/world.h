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
  void init(const std::vector<System*>& systems);
  /**
   * Create single entity
   * @return
   */
  Entity& createEntity();
  /**
   * Erase single entity
   * @param es
   */
  void eraseEntity(const Entity& es_ptr);
  /**
   * Iterate over entities with specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  EntityIterator<Ts...> scanEntities() {
    return EntityIterator<Ts...>(entities_.begin(), entities_.end());
  }
  /**
   * Iterate over specific component set
   * @tparam Ts component list
   * @return iterator & iterator proxy
   */
  template<typename... Ts>
  ComponentIterator<Ts...> scan() {
    return ComponentIterator<Ts...>(
        EntityIterator<Ts...>(entities_.begin(), entities_.end()),
        EntityIterator<Ts...>(entities_.end(), entities_.end()));
  }
  /**
   * Run simulations
   */
  void run();

 private:
  /**
   * Remove and create pending entities
   */
  void syncEntities();

// ========================== FIELDS ===============================================================

  /**
   * List of entities
   */
  std::unordered_set<Entity*> entities_;
  /**
   * List of not yet created entities
   */
  std::unordered_set<Entity*> entities_created_;
  /**
   * List of not yet deleted entities
   */
  std::vector<Entity*> entities_deleted_;
  /**
   * List of systems
   */
  std::vector<std::unique_ptr<System>> systems_;
};

}  // namespace ecs

#endif  // SRC_ECS_WORLD_H_
