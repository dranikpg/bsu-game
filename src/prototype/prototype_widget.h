#ifndef SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
#define SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_

#include <QWidget>
#include <QTimer>

#include "../ecs/ecs.h"

#include "../context/painter_context.h"
#include "../context/window_context.h"
#include "../context/input_context.h"


class PrototypeWidget : public QWidget {
 public:
  PrototypeWidget();
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;

 private:
  QTimer timer_;
  ecs::World world_;
  context::InputContext input_context_;
  context::PainterContext painter_context_;
  context::WindowContext window_context_;
};

#endif  // SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
