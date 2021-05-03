#include "typing_label.h"

#include <QDebug>

namespace ui {

TypingLabel::TypingLabel() {
  auto* timer = new QTimer(this);
  timer->setInterval(kSpeed);
  QObject::connect(timer, &QTimer::timeout, [this](){
    if (text().length() < text_) {
      QLabel::setText(text_.left(text().length() + 1));
    }
  });
  timer->start();
}

void TypingLabel::setText(const QString& text) {
  text_ = text;
  QLabel::setText("");
}

}  // namespace ui
