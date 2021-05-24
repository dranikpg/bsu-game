#ifndef SRC_WIDGETS_CLICKABLE_LABEL_H_
#define SRC_WIDGETS_CLICKABLE_LABEL_H_

#include <QLabel>
#include <QMouseEvent>

namespace ui {

class ClickableLable : public QLabel {
  Q_OBJECT

 public:
  explicit ClickableLable(QWidget* parent = 0);

 signals:
  void clicked(bool);

 protected:
  void mouseReleaseEvent(QMouseEvent* e) override {
    if (e->button() == Qt::LeftButton) {
      emit clicked(static_cast<QObject*>(this));
    }
  }
};

}  // namespace ui

#endif  // SRC_WIDGETS_CLICKABLE_LABEL_H_
