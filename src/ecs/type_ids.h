#ifndef SRC_ECS_TYPE_IDS_H_
#define SRC_ECS_TYPE_IDS_H_

#include <typeinfo>

namespace ecs {

/**
 * Assign unique id to each type for internal template conversions
 * @tparam T any type used in the ecs
 * @return its unique id
 */
template<typename T>
static std::size_t ID() {
  return typeid(T).hash_code();
}

}  // namespace ecs

#endif  // SRC_ECS_TYPE_IDS_H_
