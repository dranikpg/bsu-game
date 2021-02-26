#ifndef SRC_ECS_COMPONENT_ITERATOR_H_
#define SRC_ECS_COMPONENT_ITERATOR_H_

#include <tuple>

#include "entity_iterator.h"

namespace ecs {
/**
 * Iterator over components which <b> skips </b> entities that don't have any of the listed
 * components
 * Implements `begin()` and 'end()' to act as a proxy object
 * @tparam Ts required list of components
 */
template<typename... Ts>
class ComponentIterator {
 public:
  using iterator_category = std::forward_iterator_tag;
  using value_type = std::tuple<Ts...>;
  using difference_type = std::ptrdiff_t;

  /**
   * Construct from a set iterator
   * @param begin begin
   * @param end end
   */
  ComponentIterator(EntityIterator<Ts...> begin,
                    EntityIterator<Ts...> end);

  std::tuple<Ts& ...> operator*();

  ComponentIterator& operator++();

  ComponentIterator operator++(int);

  ComponentIterator begin();

  ComponentIterator end();

  bool operator==(const ComponentIterator<Ts...>& other) const;

  bool operator!=(const ComponentIterator<Ts...>& other) const;

 private:
  EntityIterator<Ts...> begin_iterator_;
  EntityIterator<Ts...> end_iterator_;
};

template<typename... Ts>
ComponentIterator<Ts...>::ComponentIterator(EntityIterator<Ts...> begin,
                                            EntityIterator<Ts...> end)
    : begin_iterator_(begin), end_iterator_(end) {
}

template<typename... Ts>
ComponentIterator<Ts...>& ComponentIterator<Ts...>::operator++() {
  ++begin_iterator_;
  return *this;
}

template<typename... Ts>
ComponentIterator<Ts...> ComponentIterator<Ts...>::operator++(int) {
  ComponentIterator<Ts...> copy = this;
  ++(*this);
  return copy;
}

template<typename... Ts>
ComponentIterator<Ts...> ComponentIterator<Ts...>::begin() {
  return ComponentIterator(begin_iterator_, end_iterator_);
}

template<typename... Ts>
ComponentIterator<Ts...> ComponentIterator<Ts...>::end() {
  return ComponentIterator(end_iterator_, end_iterator_);
}

template<typename... Ts>
std::tuple<Ts& ...> ComponentIterator<Ts...>::operator*() {
  Entity& e = *begin_iterator_;
  return e.Unpack<Ts...>();
}

template<typename... Ts>
bool ComponentIterator<Ts...>::operator==(const ComponentIterator<Ts...>& other) const {
  return begin_iterator_ == other.begin_iterator_;
}

template<typename... Ts>
bool ComponentIterator<Ts...>::operator!=(const ComponentIterator<Ts...>& other) const {
  return !(*this == other);
}

}  // namespace ecs

#endif  // SRC_ECS_COMPONENT_ITERATOR_H_
