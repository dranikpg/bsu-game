#include "answer_widget.h"

#include <QVBoxLayout>

namespace game {
  AnswerWidget::AnswerWidget(QWidget* parent) : QWidget(parent) {
    edit_ = new QLineEdit("Хочу питсы ... ", this);
    button_ = new QPushButton("Абоба", this);

    auto layout = new QVBoxLayout();
    layout->addWidget(edit_);
    layout->addWidget(button_);

    setLayout(layout);
  }
}  // namespace game