#include "choose_var_widget.h"

#include <utility>

#include <QHBoxLayout>
#include <QBrush>

namespace ui {

ChooseVarWidget::ChooseVarWidget(QWidget* container, const QString& text, ChooseVarWidgetType type)
: container_(container), type_(type) {
  resize(container_->size());
  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0, 0, 0, 0);
  container_->setLayout(container_layout);

  auto main_layout = new QHBoxLayout(this);

  label_ = new ClickableLable(this);
  label_->setText(text);
  label_->setWordWrap(true);
  label_->setAlignment(Qt::AlignCenter);
  label_->setObjectName("choose-var-widget-text");

  if (type_ == ChooseVarWidgetType::kAnswer) {
    background_ = QPixmap(":/choose_variant_for_chernov_A.png");
    animations_ = (utils::AseAnimationParser::Parse(
        QFile(":/animation_button_millionaire.json")))["animation"];
    connect(label_, SIGNAL(clicked(bool)), this, SIGNAL(clicked(bool)));
    connect(this, SIGNAL(clicked(bool)), this, SLOT(PlayAnimation(bool)));
  } else if (type_ == ChooseVarWidgetType::kQuestion) {
    background_ = QPixmap(":/choose_variant_for_chernov_Q.png");
  }

  current_ = background_;

  main_layout->addWidget(label_);
  main_layout->setSpacing(0);
  main_layout->setContentsMargins(0, 0, 0, 0);
  setLayout(main_layout);

  RecalculateSizes();

  show();
}

void ChooseVarWidget::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  QRect target = {0, 0, width(), height()};
  QRect source = {0, 0, current_.width(), current_.height()};
  painter.drawPixmap(target, current_, source);
}

void ChooseVarWidget::PlayAnimation(bool) {
  timer_ = new QTimer();
  timer_->setInterval(100);
  QObject::connect(timer_, &QTimer::timeout, [this](){
    static int counter = 0;
    if (animations_->GetFrameCount() > counter+1) {
      current_ = animations_->GetPixmap().copy(animations_->GetFrame(counter));
      ++counter;
      update();
    } else {
      timer_->stop();
      QTimer::singleShot(500, this, [this](){emit finishedAnimation(true);});
    }
  });

  timer_->start();
}

void ChooseVarWidget::resizeEvent(QResizeEvent* event) {
  resize(container_->size());
  RecalculateSizes();
  update();
}

void ChooseVarWidget::RecalculateSizes() {
  label_->setGeometry(static_cast<int>(0.1*width()),
                      static_cast<int>(0.14*height()),
                      static_cast<int>(0.8*width()),
                      static_cast<int>(0.72*height()));
}

}  // namespace ui
