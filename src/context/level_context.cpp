#include "level_context.h"

namespace context {

void LevelContext::SetLoadCallback(const Callback& load_callback) {
  load_callback_ = load_callback;
}

}  // namespace context
