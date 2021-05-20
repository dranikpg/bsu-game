#ifndef SRC_WIDGETS_TYPING_LABEL_H_
#define SRC_WIDGETS_TYPING_LABEL_H_

#include <QTimer>
#include <QLabel>

namespace ui {

class TypingLabel : public QLabel {
  Q_OBJECT
 public:
  using Callback = std::function<void()>;
  TypingLabel(Callback callback = [](){});
  void setText(const QString& text);

  signals:
  void TypedSymbol(int current_size);

 private:
  const int kTypingInterval = 50;
  QString text_;
  QTimer* timer_;

  Callback callback_;
};

}  // namespace ui

#endif  // SRC_WIDGETS_TYPING_LABEL_H_
