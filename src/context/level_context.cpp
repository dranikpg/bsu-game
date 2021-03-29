#include "level_context.h"

namespace context {

void LevelContext::SetOnLevelCreated(const Listener& load_callback) {
  on_level_crated_ = load_callback;
}

}  // namespace context
