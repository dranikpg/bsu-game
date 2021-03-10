#include "window_context.h"

namespace context {

const QSize& WindowContext::GetSize() const {
  return size_;
}
void WindowContext::SetSize(const QSize& size) {
  size_ = size;
}

}