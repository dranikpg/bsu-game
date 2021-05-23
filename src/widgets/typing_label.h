#ifndef SRC_WIDGETS_TYPING_LABEL_H_
#define SRC_WIDGETS_TYPING_LABEL_H_

#include <QTimer>
#include <QLabel>

namespace ui {

class TypingLabel : public QLabel {
  Q_OBJECT
 public:
  using Callback = std::function<void()>;
  explicit TypingLabel(Callback callback = [](){});
  void setText(const QString& text);
  void setTypingInterval(int interval);
  void TypeBack();
  void SetCallback(Callback callback);
  QString getCurrentText();

  signals:
  void TypedSymbol();

 private:
  int typing_interval_ = 50;
  QString text_;
  QString current_text_;
  QTimer* timer_;

  Callback callback_;
};

}  // namespace ui

#endif  // SRC_WIDGETS_TYPING_LABEL_H_
