#ifndef SRC_WIDGETS_TYPING_LABEL_H_
#define SRC_WIDGETS_TYPING_LABEL_H_

#include <QTimer>
#include <QLabel>

namespace ui {

class TypingLabel : public QLabel {
 public:
  TypingLabel();
  void setText(const QString& text);

 private:
  const int kTypingInterval = 50;
  QString text_;
};

}  // namespace ui

#endif  // SRC_WIDGETS_TYPING_LABEL_H_
