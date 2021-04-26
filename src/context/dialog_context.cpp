#include "dialog_context.h"

namespace context {

void DialogContext::Show(const utils::PixmapRect& rect) {
  container_->show();
  image_label_->setPixmap(rect.Copy());
}

void DialogContext::SetText(const QString& message) {
  text_label_->setText(message);
}

void DialogContext::SetQuestions(const DialogContext::OptionBag& options,
                                 DialogContext::AnswerHandler handler) {
  for (auto& o: options) {}
}

void DialogContext::Hide() {
  container_->hide();
}

void DialogContext::Init(QWidget* container) {
  container_ = container;
  container_->hide();
  container_->setStyleSheet("background-color: white");

  text_label_ = new QLabel(container);
  text_label_->setMinimumWidth(500);
  text_label_->move(100, 0);
  text_label_->setStyleSheet("font-size: 25px;");

  image_label_ = new QLabel(container);
  image_label_->setFixedWidth(100);
  image_label_->setFixedHeight(100);
}

}  // namespace context
