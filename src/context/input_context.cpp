#include "input_context.h"

namespace context {

void InputContext::AddKey(Qt::Key key) {
  auto mapped_key = MapKey(key);
  if (mapped_key) {
    seq_of_keys_.insert(*mapped_key);
  }
}

void InputContext::RemoveKey(Qt::Key key) {
  auto mapped_key = MapKey(key);
  if (mapped_key) {
    seq_of_keys_.erase(*mapped_key);
  }
}

std::set<constants::Keys>& InputContext::GetKeys() {
  return seq_of_keys_;
}

void InputContext::Clean() {
  seq_of_keys_ = {};
}

std::optional<Keys> InputContext::MapKey(Qt::Key key) {
  switch (key) {
    case Qt::Key::Key_W:return Keys::kUp;
    case Qt::Key::Key_A:return Keys::kLeft;
    case Qt::Key::Key_S:return Keys::kRight;
    case Qt::Key::Key_D:return Keys::kDown;
    default:return std::nullopt;
  };
}

}  // namespace context
