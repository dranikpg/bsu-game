#ifndef CHOOSE_VAR_WIDGET_H
#define CHOOSE_VAR_WIDGET_H

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QPainter>
#include <QLabel>
#include "../typing_label.h"

namespace ui {

class ChooseVarWidget : public QWidget {
  Q_OBJECT

 public:
  explicit ChooseVarWidget(QWidget* container,
                           const QString& text);
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent *event) override;
  void mousePressEvent(QMouseEvent *event) override;

  signals:
  void clicked(bool);

 private:
  void RecalculateSizes();

  QWidget* container_;
  QLabel* label_;

};

}  // namespace ui

#endif //CHOOSE_VAR_WIDGET_H
