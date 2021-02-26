#ifndef SRC_ECS_ENTITY_ITERATOR_H_
#define SRC_ECS_ENTITY_ITERATOR_H_

#include <memory>
#include <unordered_set>

#include "entity.h"

namespace ecs {

/**
 * Iterator which <b> skips </b> entities that don't have any of the listed components
 * Implements `begin()` and 'end()' to act as a proxy object
 * @tparam Ts required list of components
 */
template<typename... Ts>
class EntityIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = Entity;
  using difference_type = std::ptrdiff_t;
  using pointer = Entity*;
  using reference = Entity&;
  using world_iterator = std::unordered_set<std::shared_ptr<Entity>>::iterator;

  /**
   * Construct from a set iterator
   * @param begin begin
   * @param end end
   */
  EntityIterator(world_iterator begin,
                 world_iterator end);

  /**
   * Get single or null
   * @return sing entity pointer or null
   */
  Entity* Peek() const;

  Entity& operator*();

  Entity* operator->();

  EntityIterator& operator++();

  EntityIterator operator++(int);

  EntityIterator begin();

  EntityIterator end();

  bool operator==(const EntityIterator<Ts...>& other) const;

  bool operator!=(const EntityIterator<Ts...>& other) const;

 private:
  /**
   * @param entity
   * @return true if to include
   */
  bool Check(Entity* entity);

  /**
   * Find next entity
   * @param forced whether to force step over
   */
  void Advance(bool forced = false);

 private:
  world_iterator begin_iterator_;
  world_iterator end_iterator_;
};

template<typename... Ts>
EntityIterator<Ts...>::EntityIterator(world_iterator begin, world_iterator end)
    : begin_iterator_(begin),
      end_iterator_(end) {
  Advance();
}

template<typename... Ts>
EntityIterator<Ts...>& EntityIterator<Ts...>::operator++() {
  Advance(true);
  return *this;
}

template<typename... Ts>
EntityIterator<Ts...> EntityIterator<Ts...>::operator++(int) {
  EntityIterator copy = *this;
  Advance(true);
  return copy;
}

template<typename... Ts>
EntityIterator<Ts...> EntityIterator<Ts...>::begin() {
  return EntityIterator(begin_iterator_, end_iterator_);
}

template<typename... Ts>
EntityIterator<Ts...> EntityIterator<Ts...>::end() {
  return EntityIterator(end_iterator_, end_iterator_);
}

template<typename... Ts>
Entity& EntityIterator<Ts...>::operator*() {
  return **begin_iterator_;
}

template<typename... Ts>
Entity* EntityIterator<Ts...>::operator->() {
  return begin_iterator_->get();
}

template<typename... Ts>
bool EntityIterator<Ts...>::operator==(const EntityIterator<Ts...>& other) const {
  return begin_iterator_ == other.begin_iterator_;
}

template<typename... Ts>
bool EntityIterator<Ts...>::operator!=(const EntityIterator<Ts...>& other) const {
  return !(*this == other);
}

template<typename... Ts>
Entity* EntityIterator<Ts...>::Peek() const {
  if (begin_iterator_ == end_iterator_) {
    return nullptr;
  } else {
    return begin_iterator_->get();
  }
}

template<typename... Ts>
void EntityIterator<Ts...>::Advance(bool forced) {
  if (begin_iterator_ == end_iterator_) {
    return;
  }
  if (forced) {
    begin_iterator_++;
  }
  while (begin_iterator_ != end_iterator_ && !Check(begin_iterator_->get())) {
    begin_iterator_++;
  }
}

template<typename... Ts>
bool EntityIterator<Ts...>::Check(Entity* entity) {
  return entity->HasComponent<Ts...>();
}

}  // namespace ecs


#endif   // SRC_ECS_ENTITY_ITERATOR_H_
