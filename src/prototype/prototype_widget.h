#ifndef SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
#define SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_

#include <QWidget>
#include <QLabel>
#include <QTimer>

#include "../ecs/ecs.h"

#include "../context/painter_context.h"
#include "../context/window_context.h"
#include "../context/input_context.h"
#include "../context/level_context.h"
#include "../context/dialog_context.h"

#include "../map/map_instantiator.h"

class PrototypeWidget : public QWidget {
 public:
  PrototypeWidget();
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent *event) override;
  void keyPressEvent(QKeyEvent *event) override;
  void keyReleaseEvent(QKeyEvent *event) override;

 private:
  QTimer timer_;
  ecs::World world_;

  context::LevelContext level_context_;
  context::InputContext input_context_;
  context::PainterContext painter_context_;
  context::WindowContext window_context_;
  context::DialogContext dialog_context_;

  // UI
  QWidget dialog_box_;
};

#endif  // SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
