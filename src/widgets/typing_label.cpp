#include "typing_label.h"

#include <utility>

namespace ui {

TypingLabel::TypingLabel(Callback callback) : callback_(std::move(callback)) {
  timer_ = new QTimer(this);
  timer_->setInterval(typing_interval_);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (text().length() < text_.length()) {
      current_text_ = text_.left(text().length() + 1);
      QLabel::setText(current_text_);
      emit TypedSymbol();
    } else {
      timer_->stop();
      if (callback_ != nullptr) {
        callback_();
      }
    }
  });
  timer_->start();
}

void TypingLabel::setText(const QString& text) {
  text_ = text;
  current_text_ = "";
  QLabel::setText(current_text_);
  timer_->start();
}

void TypingLabel::setTypingInterval(int interval) {
  typing_interval_ = interval;
  timer_->setInterval(typing_interval_);
}

QString TypingLabel::getCurrentText() {
  return current_text_;
}

}  // namespace ui
