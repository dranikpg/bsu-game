#include "typing_label.h"

#include <QDebug>

namespace ui {

TypingLabel::TypingLabel(Callback callback) : callback_(callback) {
  timer_ = new QTimer(this);
  timer_->setInterval(kTypingInterval);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (text().length() < text_.length()) {
      QString message = text_.left(text().length() + 1);
      QLabel::setText(message);
      qDebug() << "symbol: " << text_.left(text().length() + 1).back();
      emit TypedSymbol(message.size());
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
  QLabel::setText("");
  timer_->start();
}

}  // namespace ui
