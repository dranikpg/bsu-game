#include "npc_dialog.h"

#include <utility>
#include <QHBoxLayout>

namespace ui {

NPCDialog::NPCDialog(std::vector<std::pair<QString, int>> parts,
                     Callback callback,
                     QWidget* container)
                     : QWidget(container),
                     container_(container),
                     callback_(std::move(callback)),
                     parts_(std::move(parts)),
                     current_part_(0),
                     current_text_(parts_[current_part_].first),
                     current_pause_(parts_[current_part_].second) {
  setStyleSheet("background-color:black;");

  label_ = new TypingLabel([this](){TypingDone();});
  label_->setObjectName("npcdialog-text");
  label_->setWordWrap(true);
  label_->setTypingInterval(120);

  main_layout_ = new QHBoxLayout();
  main_layout_->addWidget(label_);
  main_layout_->setSpacing(0);
  main_layout_->setContentsMargins(0,0,0,0);

  setLayout(main_layout_);

  auto container_layout = new QHBoxLayout();
  container_layout->addWidget(this);
  container_layout->setSpacing(0);
  container_layout->setContentsMargins(0,0,0,0);
  container_->setLayout(container_layout);

  hide();

  connect(label_, SIGNAL(TypedSymbol()), this, SLOT(CalculateSize()));
}

void NPCDialog::Start() {
  label_->setText(current_text_);
  show();
}

void NPCDialog::TypingDone() {
  QTimer::singleShot(current_pause_, this, [this](){LoadNext();});
}

void NPCDialog::LoadNext() {
  if (++current_part_ < parts_.size()) {
    current_text_ = parts_[current_part_].first;
    current_pause_ = parts_[current_part_].second;
    label_->setText(current_text_);
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
  auto newFontSizeRect = label_->fontMetrics().boundingRect(container_->rect(), (Qt::TextWordWrap) | label_->alignment(), label_->getCurrentText());
  int hint_height =
      newFontSizeRect.height();
  int hint_width =
      newFontSizeRect.width();

  setGeometry(cnt_wdth-hint_width,
              cnt_height-hint_height,
              hint_width, hint_height);
}

void NPCDialog::resizeEvent(QResizeEvent* event) {
  CalculateSize();
  update();
}

}  // namespace ui