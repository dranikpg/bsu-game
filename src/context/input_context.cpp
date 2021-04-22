#include "input_context.h"

#include <QDebug>

namespace context {

std::set<constants::Keys> InputContext::kEmptySet = {};

void InputContext::AddKey(Qt::Key key) {
  auto mapped_key = MapKey(key);
  if (mapped_key) {
    keys_.insert(*mapped_key);
    frame_keys_.insert(*mapped_key);
  }
  blocked_input_ = false;
}

void InputContext::RemoveKey(Qt::Key key) {
  auto mapped_key = MapKey(key);
  if (mapped_key) {
    keys_.erase(*mapped_key);
  }
}

const std::set<constants::Keys>& InputContext::GetKeys() const {
  if (blocked_input_) {
    return kEmptySet;
  }
  return keys_;
}

std::set<constants::Keys>& InputContext::GetFrameKeys() {
  if (blocked_input_) {
    return kEmptySet;
  }
  return frame_keys_;
}


void InputContext::Clean() {
  frame_keys_ = {};
}

std::optional<Keys> InputContext::MapKey(Qt::Key key) {
  switch (key) {
    case Qt::Key::Key_W:
        return Keys::kUp;
    case Qt::Key::Key_A:
      return Keys::kLeft;
    case Qt::Key::Key_S:
      return Keys::kDown;
    case Qt::Key::Key_D:
      return Keys::kRight;
    case Qt::Key::Key_T:
      return Keys::kEnter;
    default:
      return std::nullopt;
  }
}

void InputContext::BlockInput() {
  blocked_input_ = true;
}

}  // namespace context
