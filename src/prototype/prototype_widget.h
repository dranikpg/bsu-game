#ifndef SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
#define SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_

#include <QWidget>

#include "../ecs/ecs.h"
#include "../systems/rendering_system.h"

#include "../context/painter_context.h"


class PrototypeWidget : public QWidget {
 public:
  PrototypeWidget();
  void paintEvent(QPaintEvent *event) override;
 private:
  World world_{};
  context::PainterContext painter_context_{};
};

#endif  // SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
