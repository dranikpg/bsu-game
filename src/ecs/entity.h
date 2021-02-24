#ifndef SRC_ECS_ENTITY_H_
#define SRC_ECS_ENTITY_H_

#include <unordered_map>
#include <tuple>
#include <memory>

#include "type_ids.h"
#include "component.h"

namespace ecs {

/**
 * Basic component bag
 */
class Entity {
 public:
  /**
   * @tparam Ts List of component classes
   * @return if it contains all of them
   */
  template<typename... Ts>
  bool has();

  /**
   * Get pointer to component
   * @tparam T component class, extends `Component`
   * @return pointer to the component or null
   */
  template<typename T>
  T* get();

  /**
   * Get reference to component
   * @tparam T component class, extends `Component`
   * @throws std::invalid_argument if it does not contain the component
   * @return reference to the component
   */
  template<typename T>
  T& getRef();

  /**
   * Return tuple of component pointers
   * @tparam Ts list of component classes
   * @return tuple with pointers to listed components
   */
  template<typename... Ts>
  std::tuple<Ts* ...> unpack();

  /**
   * Return tuple of component references
   * @tparam Ts list of component classes
   * @throws std::invalid_argument is if does not contain any of the
   * listed components
   * @return tuple with reference to listed components
   */
  template<typename... Ts>
  std::tuple<Ts& ...> unpackRef();

  /**
   * Create component
   * @tparam T component class
   * @tparam Ts list of constructor arguments
   * @param args a
   * @return reference to created component
   */
  template<typename T, typename... Ts>
  Entity& create(Ts... args);

  /**
   * Remove component
   * @tparam T component class to remove
   */
  template<typename T>
  void remove();

 private:
  std::unordered_map<std::size_t, std::unique_ptr<Component>> components_;
};

// ========================== Implementation =======================================================

template<typename... Ts>
bool Entity::has() {
  bool has = true;
  ((has &= components_.find(ID<Ts>()) != components_.end()), ...);
  return has;
}

template<typename T>
T* Entity::get() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must inherit Component!");
  auto it = components_.find(ID<T>());
  if (it == components_.end()) {
    return nullptr;
  } else {
    return static_cast<T*>(it->second.get());
  }
}

template<typename T>
T& Entity::getRef() {
  T* ptr = get<T>();
  if (ptr == nullptr) {
    throw std::invalid_argument("Component class not present");
  } else {
    return *ptr;
  }
}

template<typename... Ts>
std::tuple<Ts* ...> Entity::unpack() {
  return std::make_tuple(get<Ts>()...);
}

template<typename... Ts>
std::tuple<Ts& ...> Entity::unpackRef() {
  return std::forward_as_tuple(getRef<Ts>()...);
}

template<typename T, typename... Ts>
Entity& Entity::create(Ts... args) {
  static_assert(std::is_base_of<Component, T>::value,
                "T must inherit Component!");
  components_[ID<T>()] = std::make_unique<T>(args...);
  return *this;
}

template<typename T>
void Entity::remove() {
  static_assert(std::is_base_of<Component, T>::value,
                "T must inherit Component!");
  auto it = components_.find(ID<T>());
  if (it != components_.end()) {
    components_.erase(it);
  }
}

}  // namespace ecs

#endif  // SRC_ECS_ENTITY_H_
