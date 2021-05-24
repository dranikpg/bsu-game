#include "npc_dialog.h"

#include <utility>
#include <cmath>

#include <QHBoxLayout>
#include <QPainter>
#include <QPainterPath>

#include <QDebug>

namespace ui {

NPCDialog::NPCDialog(std::vector<std::pair<QString, int>> parts,
                     Callback callback,
                     QWidget* container,
                     DialogType type)
                     : type_(type),
                     QWidget(container),
                     container_(container),
                     callback_(std::move(callback)),
                     parts_(std::move(parts)),
                     current_part_(0),
                     current_text_(parts_[current_part_].first),
                     current_pause_(parts_[current_part_].second) {

  label_ = new TypingLabel([this](){TypingDone();});
  label_->setObjectName("npcdialog-text");
  label_->setWordWrap(true);
  label_->setTypingInterval(120);

  main_layout_ = new QHBoxLayout();
  main_layout_->addWidget(label_);
  main_layout_->setSpacing(0);
  main_layout_->setContentsMargins(5,5,5,5);

  setLayout(main_layout_);

  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  if (container_->layout() != nullptr) {
    delete container_->layout();
  }
  container_->setLayout(container_layout);

  hide();

  connect(label_, SIGNAL(TypedSymbol()), this, SLOT(CalculateSize()));
}

void NPCDialog::Start() {
  qDebug() << "NPCDialog::Start";
  label_->setText(current_text_);
  show();
  TypingStart();
}

void NPCDialog::TypingDone() {
  qDebug() << "NPCDialog::TypingDone";
  TypingEnd();
  QTimer::singleShot(current_pause_, this, [this](){LoadNext();});
}

void NPCDialog::LoadNext() {
  if (++current_part_ < parts_.size()) {
    current_text_ = parts_[current_part_].first;
    current_pause_ = parts_[current_part_].second;
    label_->setText(current_text_);
    TypingStart();
  } else {
    hide();
    if (callback_ != nullptr) {
      callback_();
    }
  }
}

void NPCDialog::CalculateSize() {
  int cnt_wdth = container_->width();
  int cnt_height = container_->height();
  auto newFontSizeRect = label_->fontMetrics().boundingRect(
      QRect{0,0,(container_->width() - 10), (container_->height() - 10)},
      (Qt::TextWordWrap) | label_->alignment(),
      label_->getCurrentText());
  int hint_height =
      newFontSizeRect.height()+10;
  int hint_width =
      newFontSizeRect.width()+10;

  if (type_ == DialogType::kLeftBottom) {
    setGeometry(0,
                cnt_height-hint_height,
                hint_width, hint_height);
  } else if (type_ == DialogType::kRightBottom) {
    setGeometry(cnt_wdth-hint_width,
                cnt_height-hint_height,
                hint_width, hint_height);
  }
}

void NPCDialog::resizeEvent(QResizeEvent* event) {
  CalculateSize();
  update();
}

void NPCDialog::paintEvent(QPaintEvent* event) {
  QPainter painter(this);
  painter.setRenderHint(QPainter::Antialiasing);
  QPainterPath path;
  int dialog_width = std::min(container_->width(), width());
  int dialog_height = std::min(container_->height(), height());
  int dialog_x = std::max(0, width() - container_->width());
  int dialog_y = std::max(0, height() - container_->height());
  path.addRoundedRect(QRectF(dialog_x, dialog_y,
                             dialog_width, dialog_height), 10, 10);
  QPen pen(Qt::transparent, 1);
  painter.setPen(pen);
  painter.fillPath(path, Qt::white);
  painter.drawPath(path);
}

void NPCDialog::TypingStart() {
  if (typing_start_callback_ != nullptr) {
    typing_start_callback_();
  }
}

void NPCDialog::TypingEnd() {
  if (typing_end_callback_ != nullptr) {
    typing_end_callback_();
  }
}

void NPCDialog::SetTypingStartCallback(Callback callback) {
  typing_start_callback_ = std::move(callback);
}

void NPCDialog::SetTypingEndCallback(Callback callback) {
  typing_end_callback_ = std::move(callback);
}

}  // namespace ui