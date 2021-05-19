#ifndef ANSWER_WIDGET_H
#define ANSWER_WIDGET_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>

namespace game {

  class AnswerWidget : public QWidget {
   public:
    AnswerWidget(QWidget* parent);
   private:
    QLineEdit* edit_;
    QPushButton* button_;
  };

}  // namespace game

#endif //ANSWER_WIDGET_H
