#include "choose_var_widget.h"

#include <utility>

namespace ui {

ChooseVarWidget::ChooseVarWidget(QWidget* container) {
  container_ = container;
  setParent(container_);
}

void ChooseVarWidget::Start(Callback callback, const QString& text) {
  callback_ = std::move(callback);
  text_ = text;

}

}  // namespace ui