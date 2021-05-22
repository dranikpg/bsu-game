#include "choose_widget.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>

namespace ui {

ChooseWidget::ChooseWidget(QWidget* container) : QWidget(container),
                                                container_(container) {
  resize(container_->size());
}

void ChooseWidget::Start(const QString& question,
                         const QString& var1,
                         const QString& var2,
                         const QString& var3,
                         const QString& var4,
                         Callback callback,
                         int* chosen_var_ret) {
  chosen_var_ret_ = chosen_var_ret;

  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  callback_ = std::move(callback);
  question_label_ = new TypingLabel([this](){ShowButtons();});
  var_1_but_ = new QPushButton(var1, this);
  var_2_but_ = new QPushButton(var2, this);
  var_3_but_ = new QPushButton(var3, this);
  var_4_but_ = new QPushButton(var4, this);
  var_1_but_->setWindowOpacity(0.3);
  // var_1_but_->hide();
  // var_2_but_->hide();
  // var_3_but_->hide();
  // var_4_but_->hide();
  question_label_->setParent(this);

  question_label_->setText(question);
  qDebug() << this->parent();
  RecalculateSizes();
  show();
}

void ChooseWidget::ShowButtons() {
  qDebug() << "ChooseWidget::ShowButtons";


  connect(var_1_but_, SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  connect(var_2_but_, SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  connect(var_3_but_, SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  connect(var_4_but_, SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));
}

void ChooseWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseWidget::RecalculateSizes() {
  qDebug() << "ChooseWidget::RecalculateSizes";
  var_1_but_->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_2_but_->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_3_but_->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_4_but_->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  question_label_->setGeometry(static_cast<int>(0.0*container_->width()),
                          static_cast<int>(0.0*container_->height()),
                          static_cast<int>(1.0*container_->width()),
                          static_cast<int>(0.2*container_->height()));
}

void ChooseWidget::Var1ButClicked(bool) {
  qDebug() << "ChooseWidget::Var1ButClicked";
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 1;
  }
  End();
}

void ChooseWidget::Var2ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 2;
  }
  End();
}

void ChooseWidget::Var3ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 3;
  }
  End();
}

void ChooseWidget::Var4ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 4;
  }
  End();
}

void ChooseWidget::End() {
  qDebug() << "ChooseWidget::End";
  hide();
  if (callback_) {
    callback_();
  }
}

}  // namespace ui