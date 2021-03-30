#include "level_context.h"

namespace context {

void LevelContext::SetOnLevelCreated(const Listener& listener) {
  on_level_created_ = listener;
}

}  // namespace context
