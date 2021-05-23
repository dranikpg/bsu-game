#ifndef CLICKABLE_LABEL_H
#define CLICKABLE_LABEL_H

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
        emit clicked((QObject*) this);
      }
    }

  };
}  // namespace ui



#endif //CLICKABLE_LABEL_H
