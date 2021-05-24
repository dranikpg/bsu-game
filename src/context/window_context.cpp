#include "window_context.h"

namespace context {

const QSize& WindowContext::GetSize() const {
  return size_;
}

void WindowContext::SetSize(const QSize& size) {
  size_ = size;
}

const QTransform& WindowContext::GetRenderTransform() const {
  return render_transform_;
}
void WindowContext::SetRenderTransform(const QTransform& render_transform) {
  render_transform_ = render_transform;
}

}  // namespace context
