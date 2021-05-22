#ifndef CHOOSE_VAR_WIDGET_H
#define CHOOSE_VAR_WIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QPainter>
#include "../typing_label.h"

namespace ui {

class ChooseVarWidget : public QWidget {
  Q_OBJECT

 public:
  using Callback = std::function<void()>;
  explicit ChooseVarWidget(QWidget* container, Callback callback,
                           QString text);
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

  void Start();

  signals:
  void clicked(bool);

 private:
  void End();

  Callback callback_;
  QString text_;
  QWidget* container_;
  QTimer* timer_;
  ui::TypingLabel* label_;

  int current_alpha_ = 0;
};

}  // namespace ui

#endif //CHOOSE_VAR_WIDGET_H
