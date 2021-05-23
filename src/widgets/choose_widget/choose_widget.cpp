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
  question_text_ = question;
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

  var_1_ = new ChooseVarWidget(var_1_container_, [this](){var_2_->Start();}, [this](){var_2_->Hide();}, var1);
  var_2_ = new ChooseVarWidget(var_2_container_, [this](){var_3_->Start();}, [this](){var_3_->Hide();}, var2);
  var_3_ = new ChooseVarWidget(var_3_container_, [this](){var_4_->Start();}, [this](){var_4_->Hide();}, var3);
  var_4_ = new ChooseVarWidget(var_4_container_, [this](){MakeConnections();}, [this](){HideMask();}, var4);
  vars_.push_back(var_1_);
  vars_.push_back(var_2_);
  vars_.push_back(var_3_);
  vars_.push_back(var_4_);

  question_label_ = new TypingLabel([this](){ShowVars();});
  question_label_->setParent(this);

  RecalculateSizes();
  show();

  ShowMask();
}

void ChooseWidget::ShowMask() {
  timer_ = new QTimer(this);
  timer_->setInterval(35);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (current_alpha_ > 150) {
      current_alpha_ = 170;
      timer_->stop();
      ShowQuestion();
    } else {
      current_alpha_ += 13;
      update();
    }
  });

  timer_->start();
}

void ChooseWidget::HideMask() {
  timer_ = new QTimer(this);
  timer_->setInterval(35);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (current_alpha_ < 20) {
      current_alpha_ = 0;
      timer_->stop();
      End();
    } else {
      current_alpha_ -= 13;
      update();
    }
  });

  timer_->start();
}

void ChooseWidget::ShowQuestion() {
  question_label_->setText(question_text_);
}

void ChooseWidget::HideQuestion() {
  qDebug() << "ChooseWidget::HideQuestion";
  question_label_->SetCallback([this](){HideVars();});
  question_label_->TypeBack();
}

void ChooseWidget::HideVars() {
  qDebug() << "ChooseWidget::HideVars";
  var_1_->Hide();
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
  QPainter painter(this);
  painter.fillRect(rect(), QBrush(QColor(220,220,220,current_alpha_)));
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

void ChooseWidget::Hide() {
  qDebug() << "ChooseWidget::Hide";
  disconnect(var_1_, SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  disconnect(var_2_, SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  disconnect(var_3_, SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  disconnect(var_4_, SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));
  HideQuestion();
}

void ChooseWidget::Var1ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 0;
  }
  Hide();
}

void ChooseWidget::Var2ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 1;
  }
  Hide();
}

void ChooseWidget::Var3ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 2;
  }
  Hide();
}

void ChooseWidget::Var4ButClicked(bool) {
  if (chosen_var_ret_ != nullptr) {
    *chosen_var_ret_ = 3;
  }
  Hide();
}

void ChooseWidget::End() {
  hide();
  if (callback_) {
    callback_();
  }
}

}  // namespace ui