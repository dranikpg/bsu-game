#include "input_context.h"

namespace context {

void InputContext::AddKey(Qt::Key key) {
  switch (key) {
    case Qt::Key::Key_W:
      seq_of_keys_.insert(Keys::kUp);
      break;
    case Qt::Key::Key_A:
      seq_of_keys_.insert(Keys::kLeft);
      break;
    case Qt::Key::Key_S:
      seq_of_keys_.insert(Keys::kDown);
      break;
    case Qt::Key::Key_D:
      seq_of_keys_.insert(Keys::kRight);
      break;
    default:
      break;
  }
}

std::set<constants::Keys>& InputContext::GetKeys() {
  return seq_of_keys_;
}

void InputContext::Clean() {
  seq_of_keys_ = {};
}

}  // namespace context
