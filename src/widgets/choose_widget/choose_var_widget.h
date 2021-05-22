#ifndef CHOOSE_VAR_WIDGET_H
#define CHOOSE_VAR_WIDGET_H

#include <QWidget>
#include <QString>

namespace ui {

class ChooseVarWidget : public QWidget {
 public:
  using Callback = std::function<void()>;
  explicit ChooseVarWidget(QWidget* container);

  void Start(Callback callback,
             const QString& text);

 private:
  Callback callback_;
  QString text_;
  QWidget* container_;
};

}  // namespace ui

#endif //CHOOSE_VAR_WIDGET_H
