#include "choose_widget.h"

#include <QHBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QGraphicsColorizeEffect>
#include <QPropertyAnimation>
#include <QSequentialAnimationGroup>
#include <QParallelAnimationGroup>
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
  icon_pixmap = QPixmap(":/millionaire_icon.png");

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

  dark_mask_ = new QWidget(this);
  dark_mask_->setStyleSheet("background-color: black");
  dark_mask_->hide();

  icon_ = new QLabel(this);
  icon_->setPixmap(icon_pixmap.scaled(icon_->width(), icon_->height()));
  icon_->hide();

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
  question_widget_ = new ChooseVarWidget(question_widget_container_,
                                         question,
                                         ChooseVarWidget::ChooseVarWidgetType::kQuestion);
  question_widget_->hide();

  hide();
}

void ChooseWidget::Start(QPixmap start_frame) {
  start_frame_ = start_frame;
  RecalculateSizes();
  show();

  ShowMask();
}

void ChooseWidget::ShowMask() {
  mask_->setPixmap(start_frame_.scaled(mask_->width(), mask_->height()));

  auto group = new QParallelAnimationGroup;

  auto eff1 = new QGraphicsColorizeEffect(this);
  eff1->setColor(QColor::fromRgb(0,0,0));
  eff1->setStrength(0);
  mask_->setGraphicsEffect(eff1);
  mask_->show();

  auto anim1 = new QPropertyAnimation(eff1, "strength");
  anim1->setDuration(1500);
  anim1->setStartValue(0);
  anim1->setEndValue(1);
  anim1->setEasingCurve(QEasingCurve::InBack);
  group->addAnimation(anim1);

  auto eff2 = new QGraphicsOpacityEffect(this);
  eff2->setOpacity(0);
  dark_mask_->setGraphicsEffect(eff2);
  dark_mask_->show();

  auto anim2 = new QPropertyAnimation(eff2, "opacity");
  anim2->setDuration(1500);
  anim2->setStartValue(0);
  anim2->setEndValue(0.65);
  anim2->setEasingCurve(QEasingCurve::InBack);
  group->addAnimation(anim2);

  auto eff3 = new QGraphicsOpacityEffect(this);
  eff3->setOpacity(0);
  icon_->setGraphicsEffect(eff3);
  icon_->show();

  auto anim3 = new QPropertyAnimation(eff3, "opacity");
  anim3->setDuration(1500);
  anim3->setStartValue(0);
  anim3->setEndValue(1);
  anim3->setEasingCurve(QEasingCurve::InBack);
  group->addAnimation(anim3);

  group->start(QAbstractAnimation::DeleteWhenStopped);

  connect(group, SIGNAL(finished()), this, SLOT(ShowQuestion()));
}

void ChooseWidget::ShowQuestion() {
  auto eff = new QGraphicsOpacityEffect(this);
  eff->setOpacity(0);
  question_widget_->setGraphicsEffect(eff);
  question_widget_->show();
  auto anim = new QPropertyAnimation(eff, "opacity");
  anim->setDuration(1200);
  anim->setStartValue(0);
  anim->setEndValue(1);
  anim->setEasingCurve(QEasingCurve::InBack);
  anim->start(QAbstractAnimation::DeleteWhenStopped);

  connect(anim, SIGNAL(finished()), this, SLOT(ShowVars()));
}

void ChooseWidget::ShowVars() {
  auto group = new QSequentialAnimationGroup;
  for (auto& elem : vars_) {
    auto eff = new QGraphicsOpacityEffect(this);
    eff->setOpacity(0);
    elem->setGraphicsEffect(eff);
    elem->show();
    auto anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(500);
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
  for (auto& elem : vars_) {
    connect(elem, SIGNAL(finishedAnimation(bool)), this, SLOT(Hide()));
  }
}


void ChooseWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseWidget::RecalculateSizes() {
  vars_containers_[0]->setGeometry(static_cast<int>(0.125*container_->width()),
                          static_cast<int>(0.73*container_->height()),
                          static_cast<int>(0.37*container_->width()),
                          static_cast<int>(0.10*container_->height()));
  vars_containers_[1]->setGeometry(static_cast<int>(0.5*container_->width()),
                          static_cast<int>(0.73*container_->height()),
                          static_cast<int>(0.37*container_->width()),
                          static_cast<int>(0.10*container_->height()));
  vars_containers_[2]->setGeometry(static_cast<int>(0.125*container_->width()),
                          static_cast<int>(0.84*container_->height()),
                          static_cast<int>(0.37*container_->width()),
                          static_cast<int>(0.10*container_->height()));
  vars_containers_[3]->setGeometry(static_cast<int>(0.5*container_->width()),
                          static_cast<int>(0.84*container_->height()),
                          static_cast<int>(0.37*container_->width()),
                          static_cast<int>(0.10*container_->height()));
  question_widget_container_->setGeometry(static_cast<int>(0.09*container_->width()),
                              static_cast<int>(0.57*container_->height()),
                              static_cast<int>(0.825*container_->width()),
                              static_cast<int>(0.14*container_->height()));
  icon_->setGeometry(static_cast<int>(0.27*container_->width()),
                     static_cast<int>(0.0*container_->height()),
                     static_cast<int>(0.425*container_->width()),
                     static_cast<int>(0.54*container_->height()));
  icon_->setPixmap(icon_pixmap.scaled(icon_->width(), icon_->height()));

  mask_->setGeometry(0, 0, width(), height());
  dark_mask_->setGeometry(0,0,width(),height());
  mask_->setPixmap(start_frame_.scaled(mask_->width(), mask_->height()));
}

void ChooseWidget::Hide() {
  auto group = new QParallelAnimationGroup;

  for (auto& elem : vars_) {
    auto eff = new QGraphicsOpacityEffect(this);
    eff->setOpacity(1);
    elem->setGraphicsEffect(eff);
    auto anim = new QPropertyAnimation(eff, "opacity");
    anim->setDuration(1500);
    anim->setStartValue(1);
    anim->setEndValue(0);
    anim->setEasingCurve(QEasingCurve::OutBack);
    group->addAnimation(anim);
  }

  auto anim = new QPropertyAnimation(mask_->graphicsEffect(), "strength");
  anim->setDuration(1500);
  anim->setStartValue(1);
  anim->setEndValue(0);
  anim->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim);

  auto eff = new QGraphicsOpacityEffect(this);
  eff->setOpacity(1);
  question_widget_->setGraphicsEffect(eff);
  auto anim1 = new QPropertyAnimation(eff, "opacity");
  anim1->setDuration(1500);
  anim1->setStartValue(1);
  anim1->setEndValue(0);
  anim1->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim1);

  auto anim2 = new QPropertyAnimation(dark_mask_->graphicsEffect(), "opacity");
  anim2->setDuration(1500);
  anim2->setStartValue(0.65);
  anim2->setEndValue(0);
  anim2->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim2);

  auto anim3 = new QPropertyAnimation(icon_->graphicsEffect(), "opacity");
  anim3->setDuration(1500);
  anim3->setStartValue(1);
  anim3->setEndValue(0);
  anim3->setEasingCurve(QEasingCurve::OutBack);
  group->addAnimation(anim3);

  group->start(QAbstractAnimation::DeleteWhenStopped);

  connect(group, SIGNAL(finished()), this, SLOT(End()));
}

void ChooseWidget::Disconnect() {
  disconnect(vars_[0], SIGNAL(clicked(bool)), this, SLOT(Var1ButClicked(bool)));
  disconnect(vars_[1], SIGNAL(clicked(bool)), this, SLOT(Var2ButClicked(bool)));
  disconnect(vars_[2], SIGNAL(clicked(bool)), this, SLOT(Var3ButClicked(bool)));
  disconnect(vars_[3], SIGNAL(clicked(bool)), this, SLOT(Var4ButClicked(bool)));
}

void ChooseWidget::Var1ButClicked(bool) {
  chosen_var_ = 0;
  Disconnect();
}

void ChooseWidget::Var2ButClicked(bool) {
  chosen_var_ = 1;
  Disconnect();
}

void ChooseWidget::Var3ButClicked(bool) {
  chosen_var_ = 2;
  Disconnect();
}

void ChooseWidget::Var4ButClicked(bool) {
  chosen_var_ = 3;
  Disconnect();
}

void ChooseWidget::End() {
  hide();
  delete layout();
  if (callback_) {
    callback_(chosen_var_);
  }
}

}  // namespace ui