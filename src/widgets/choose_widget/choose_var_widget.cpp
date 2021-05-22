#include "choose_var_widget.h"

#include <utility>

#include <QHBoxLayout>
#include <QDebug>
#include <QBrush>

namespace ui {

ChooseVarWidget::ChooseVarWidget(QWidget* container,
                                 Callback callback,
                                 QString  text)
                                 : text_(std::move(text)),
                                 callback_(std::move(callback)) {
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

  label_ = new ui::TypingLabel([this](){End();});
  label_->setObjectName("splash-text");
  label_->setWordWrap(true);

  main_layout->addWidget(label_, Qt::AlignCenter);

  hide();
}

void ChooseVarWidget::Start() {
  timer_ = new QTimer(this);
  timer_->setInterval(35);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    if (current_alpha_ > 230) {
      current_alpha_ = 255;
      timer_->stop();
      label_->setText(text_);
      // End();
    } else {
      current_alpha_ += 13;
      update();
    }
  });

  show();
  timer_->start();
}

void ChooseVarWidget::End() {
  if (callback_ != nullptr) {
    callback_();
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

}  // namespace ui