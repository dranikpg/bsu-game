#include "choose_var_widget.h"

#include <utility>

#include <QHBoxLayout>
#include <QDebug>
#include <QBrush>

namespace ui {

ChooseVarWidget::ChooseVarWidget(QWidget* container, const QString& text) : container_(container) {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0,0,0,0);

  label_ = new QLabel(text, this);
  label_->setObjectName("splash-text");
  label_->setWordWrap(true);
  setStyleSheet("background-color: red");

  main_layout->addWidget(label_);
  setLayout(main_layout);

  RecalculateSizes();

  show();
}

void ChooseVarWidget::paintEvent(QPaintEvent* event) {

}

void ChooseVarWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseVarWidget::mousePressEvent(QMouseEvent* event) {
  emit clicked(true);
}

void ChooseVarWidget::RecalculateSizes() {
  qDebug() << "ChooseVarWidget::RecalculateSizes";
  label_->setGeometry(static_cast<int>(0.1*width()),
                      static_cast<int>(0.1*height()),
                      static_cast<int>(0.8*width()),
                      static_cast<int>(0.8*height()));
  qDebug() << label_->size() << size();
}

}  // namespace ui