#include "choose_widget.h"

#include <QHBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
#include <QDebug>
#include <utility>

namespace ui {

ChooseWidget::ChooseWidget(QWidget* container,
                           const QString&  question,
                           const QString& var1,
                           const QString& var2,
                           const QString& var3,
                           const QString& var4,
                           Callback callback) :
                           QWidget(container),
                           container_(container),
                           callback_(std::move(callback))
                           {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0,0,0,0);
  setLayout(main_layout);

  mask_ = new QLabel(this);
  mask_->hide();

  vars_containers_ = std::vector<QWidget*>({
    new QWidget(this),
    new QWidget(this),
    new QWidget(this),
    new QWidget(this)
  });


  vars_ = std::vector<ChooseVarWidget*>({
    new ChooseVarWidget(vars_containers_[0], var1),
    new ChooseVarWidget(vars_containers_[1], var2),
    new ChooseVarWidget(vars_containers_[2], var3),
    new ChooseVarWidget(vars_containers_[3], var4)
  });


  for (auto& elem : vars_) {
    elem->hide();
  }

  question_widget_container_ = new QWidget(this);
  question_widget_ = new ChooseVarWidget(question_widget_container_, question);
  question_widget_->hide();

  hide();
}

void ChooseWidget::Start(QPixmap start_frame) {
  start_frame_ = std::move(start_frame);
  RecalculateSizes();
  show();

  ShowMask();
}

void ChooseWidget::ShowMask() {
  mask_->setPixmap(start_frame_);

  auto eff = new QGraphicsColorizeEffect(this);
  eff->setColor(QColor::fromRgb(0,0,0));
  eff->setStrength(0);
  mask_->setGraphicsEffect(eff);
  mask_->show();

  auto anim = new QPropertyAnimation(eff, "strength");
  anim->setDuration(2000);
  anim->setStartValue(0);
  anim->setEndValue(1);
  anim->setEasingCurve(QEasingCurve::InBack);
  anim->start(QAbstractAnimation::DeleteWhenStopped);

  connect(anim, SIGNAL(finished()), this, SLOT(ShowQuestion()));
}

void ChooseWidget::ShowQuestion() {
  auto eff = new QGraphicsOpacityEffect(this);
  eff->setOpacity(0);
  question_widget_->setGraphicsEffect(eff);
  question_widget_->show();
  auto anim = new QPropertyAnimation(eff, "opacity");
  anim->setDuration(1000);
  anim->setStartValue(0);
  anim->setEndValue(1);
  anim->setEasingCurve(QEasingCurve::InBack);
  anim->start(QAbstractAnimation::DeleteWhenStopped);

  connect(anim, SIGNAL(finished()), this, SLOT(ShowVars()));
}

void ChooseWidget::ShowVars() {
  qDebug() << "ChooseWidget::ShowVars";
  auto group = new QSequentialAnimationGroup;
  for (auto& elem : vars_) {
    auto eff = new QGraphicsOpacityEffect(this);
    eff->setOpacity(0);
    elem->setGraphicsEffect(eff);
    elem->show();
    auto anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(1000);
    anim->setStartValue(0);
    anim->setEndValue(1);
    anim->setEasingCurve(QEasingCurve::InBack);
    group->addAnimation(anim);
  }

  group->start(QAbstractAnimation::DeleteWhenStopped);

  connect(group, SIGNAL(finished()), this, SLOT(MakeConnections()));
}

void ChooseWidget::MakeConnections() {
  connect(vars_[0], SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  connect(vars_[1], SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  connect(vars_[2], SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  connect(vars_[3], SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));
}


void ChooseWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseWidget::RecalculateSizes() {
  vars_containers_[0]->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  vars_containers_[1]->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.5*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  vars_containers_[2]->setGeometry(static_cast<int>(0.2*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  vars_containers_[3]->setGeometry(static_cast<int>(0.6*container_->width()),
                          static_cast<int>(0.8*container_->height()),
                          static_cast<int>(0.3*container_->width()),
                          static_cast<int>(0.2*container_->height()));
  question_widget_container_->setGeometry(static_cast<int>(0.0*container_->width()),
                              static_cast<int>(0.0*container_->height()),
                              static_cast<int>(1.0*container_->width()),
                              static_cast<int>(0.2*container_->height()));
  mask_->setGeometry(0, 0, width(), height());
  mask_->setPixmap(start_frame_.scaled(mask_->width(), mask_->height()));
}

void ChooseWidget::Hide() {
  qDebug() << "ChooseWidget::Hide";
  disconnect(vars_[0], SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  disconnect(vars_[1], SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  disconnect(vars_[2], SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  disconnect(vars_[3], SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));

  auto group = new QParallelAnimationGroup;

  for (auto& elem : vars_) {
    auto eff = new QGraphicsOpacityEffect(this);
    eff->setOpacity(1);
    elem->setGraphicsEffect(eff);
    auto anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(1000);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    group->addAnimation(anim);
  }

  auto anim = new QPropertyAnimation(mask_->graphicsEffect(), "strength");
  anim->setDuration(2000);
  anim->setStartValue(1);
  anim->setEndValue(0);
  anim->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim);

  auto eff = new QGraphicsOpacityEffect(this);
  eff->setOpacity(1);
  question_widget_->setGraphicsEffect(eff);
  auto anim1 = new QPropertyAnimation(eff, "opacity");
  anim1->setDuration(1000);
  anim1->setStartValue(1);
  anim1->setEndValue(0);
  anim1->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim1);

  group->start(QAbstractAnimation::DeleteWhenStopped);

  connect(group, SIGNAL(finished()), this, SLOT(End()));
}

void ChooseWidget::Var1ButClicked(bool) {
  chosen_var_ = 0;
  Hide();
}

void ChooseWidget::Var2ButClicked(bool) {
  chosen_var_ = 1;
  Hide();
}

void ChooseWidget::Var3ButClicked(bool) {
  chosen_var_ = 2;
  Hide();
}

void ChooseWidget::Var4ButClicked(bool) {
  chosen_var_ = 3;
  Hide();
}

void ChooseWidget::End() {
  hide();
  if (callback_) {
    callback_(chosen_var_);
  }
}

}  // namespace ui