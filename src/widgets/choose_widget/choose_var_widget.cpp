#include "choose_var_widget.h"

#include <utility>

#include <QHBoxLayout>
#include <QDebug>
#include <QBrush>

namespace ui {

ChooseVarWidget::ChooseVarWidget(QWidget* container,
                                 Callback constructed_callback,
                                 Callback destroyed_callback,
                                 QString  text)
                                 : text_(std::move(text)),
                                 constructed_callback_(std::move(constructed_callback)),
                                 destroyed_callback_(std::move(destroyed_callback)) {
  container_ = container;
  setParent(container_);
  resize(container_->size());

  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(5,5,5,5);
  setLayout(main_layout);

  label_ = new ui::TypingLabel();
  label_->setObjectName("splash-text");
  label_->setWordWrap(true);

  main_layout->addWidget(label_, Qt::AlignCenter);

  hide();
}

void ChooseVarWidget::Start() {
  ShowRect();
  show();
}

void ChooseVarWidget::Hide() {
  qDebug() << "ChooseVarWidget::Hide";
  is_active_ = false;
  HideText();
}

void ChooseVarWidget::ShowRect() {
  timer_ = new QTimer(this);
  timer_->setInterval(35);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (current_alpha_ > 230) {
      current_alpha_ = 255;
      timer_->stop();
      ShowText();
    } else {
      current_alpha_ += 13;
      update();
    }
  });

  timer_->start();
}

void ChooseVarWidget::HideRect() {
  qDebug() << "ChooseVarWidget::HideRect";
  timer_ = new QTimer(this);
  timer_->setInterval(35);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (current_alpha_ < 20) {
      current_alpha_ = 0;
      timer_->stop();
      EndDestruction();
    } else {
      current_alpha_ -= 13;
      update();
    }
  });

  timer_->start();
}

void ChooseVarWidget::ShowText() {
  label_->SetCallback([this](){EndConstruction();});
  label_->setText(text_);
}

void ChooseVarWidget::HideText() {
  qDebug() << "ChooseVarWidget::HideText";
  label_->SetCallback([this](){HideRect();});
  label_->TypeBack();
}

void ChooseVarWidget::EndConstruction() {
  if (constructed_callback_ != nullptr) {
    constructed_callback_();
  }
}

void ChooseVarWidget::EndDestruction() {
  if (destroyed_callback_ != nullptr) {
    destroyed_callback_();
  }
}

void ChooseVarWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.fillRect(rect(), QBrush(QColor(255,0,0,current_alpha_)));
}

void ChooseVarWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  update();
}

void ChooseVarWidget::mousePressEvent(QMouseEvent* event) {
  emit clicked(true);
}

bool ChooseVarWidget::IsActive() {
  return is_active_;
}

void ChooseVarWidget::SetDestroyedCallback(Callback callback) {
  destroyed_callback_ = std::move(callback);
}

}  // namespace ui