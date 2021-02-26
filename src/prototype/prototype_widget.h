#ifndef SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
#define SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_

#include <QWidget>

class PrototypeWidget : public QWidget {
 public:
  void paintEvent(QPaintEvent *event) override;
};

#endif  // SRC_PROTOTYPE_PROTOTYPE_WIDGET_H_
