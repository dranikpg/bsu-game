#include "level_context.h"

namespace context {

void LevelContext::SetLoadCallback(const Callback& load_callback) {
  loadCallback_ = load_callback;
}

}  // namespace context
