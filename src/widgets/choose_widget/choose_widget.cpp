#include "choose_widget.h"

#include <QHBoxLayout>
#include <QTimer>
#include <QDebug>

namespace ui {

ChooseWidget::ChooseWidget(QWidget* container) : QWidget(container),
                                                container_(container) {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);
}

void ChooseWidget::Start(const QString& question,
                         const QString& var1,
                         const QString& var2,
                         const QString& var3,
                         const QString& var4,
                         Callback callback,
                         int* chosen_var_ret) {
  chosen_var_ret_ = chosen_var_ret;

  callback_ = std::move(callback);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(5,5,5,5);
  setLayout(main_layout);

  var_1_container_ = new QWidget(this);
  var_2_container_ = new QWidget(this);
  var_3_container_ = new QWidget(this);
  var_4_container_ = new QWidget(this);

  var_1_ = new ChooseVarWidget(var_1_container_, [this](){var_2_->Start();}, var1);
  var_2_ = new ChooseVarWidget(var_2_container_, [this](){var_3_->Start();}, var2);
  var_3_ = new ChooseVarWidget(var_3_container_, [this](){var_4_->Start();}, var3);
  var_4_ = new ChooseVarWidget(var_4_container_, [this](){MakeConnections();}, var4);

  question_label_ = new TypingLabel([this](){ShowVars();});
  question_label_->setParent(this);

  RecalculateSizes();
  show();

  question_label_->setText(question);
}

void ChooseWidget::ShowVars() {
  var_1_->Start();
}

void ChooseWidget::MakeConnections() {
  connect(var_1_, SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  connect(var_2_, SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  connect(var_3_, SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  connect(var_4_, SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));
}

void ChooseWidget::paintEvent(QPaintEvent* event) {
  qDebug() << "paint event";
  QPainter painter(this);
  painter.fillRect(rect(), QBrush(QColor(255,255,0,current_alpha_)));
}

void ChooseWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseWidget::RecalculateSizes() {
  var_1_container_->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_2_container_->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_3_container_->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  var_4_container_->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  question_label_->setGeometry(static_cast<int>(0.0*container_->width()),
                          static_cast<int>(0.0*container_->height()),
                          static_cast<int>(1.0*container_->width()),
                          static_cast<int>(0.2*container_->height()));
}

void ChooseWidget::Var1ButClicked(bool) {
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
  hide();
  if (callback_) {
    callback_();
  }
}

}  // namespace ui