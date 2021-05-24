#include "splash_context.h"

#include <utility>

#include <QFont>

namespace context {

void SplashContext::Init(QWidget* container) {
  container_ = container;
  container_->setObjectName("splash");
  container_->hide();

  main_layout_ = new QGridLayout();
  container_->setLayout(main_layout_);

  text_ = new ui::TypingLabel();
  text_->setObjectName("splash-text");
  text_->setWordWrap(true);

  button_ = new QPushButton("OK");
  button_->setObjectName("splash-button");
  QWidget::connect(button_, &QPushButton::clicked, [this]() {
    if (callback_) {
      callback_();
      callback_ = std::function<void()>();
    }
    container_->hide();
  });

  icon_ = new QLabel();

  main_layout_->addWidget(icon_, 0, 0, 2, 1, Qt::AlignCenter);
  main_layout_->addWidget(text_, 0, 1, 1, 1, Qt::AlignCenter);
  main_layout_->addWidget(button_, 1, 1, 1, 1, Qt::AlignCenter);
  main_layout_->setColumnStretch(0, 1);
  main_layout_->setColumnStretch(1, 2);
  icon_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

void SplashContext::Show(utils::PixmapRect icon, QString text, std::function<void()> callback) {
  icon_->setPixmap(icon.Copy());
  text_->setText(text);
  container_->show();
  if (callback_) {
    callback_();
  }
  callback_ = std::move(callback);
}

}  // namespace context
