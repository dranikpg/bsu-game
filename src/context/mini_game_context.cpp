#include "mini_game_context.h"

namespace context {

void context::MiniGameContext::Start() {
  active_ = true;
}

void MiniGameContext::Stop() {
  active_ = false;
  // force delete all children
  container_->setUpdatesEnabled(false);
  qDeleteAll(container_->findChildren<QWidget*>("", Qt::FindDirectChildrenOnly));
  container_->setUpdatesEnabled(true);
}

void MiniGameContext::Init(QWidget* container) {
  container_ = container;
}

QWidget* MiniGameContext::GetContainer() const {
  return container_;
}
const QTransform& MiniGameContext::GetRenderTransform() const {
  return render_transform_;
}
void MiniGameContext::SetRenderTransform(const QTransform& render_transform) {
  render_transform_ = render_transform;
}

}  // namespace context
