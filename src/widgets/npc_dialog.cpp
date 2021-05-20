#include "npc_dialog.h"

#include <QDebug>
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

  connect(label_, SIGNAL(TypedSymbol(int)), this, SLOT(CalculateSize(int)));
  qDebug() << "NPCDialog::NPCDialog";
}

void NPCDialog::Start() {
  label_->setText(current_text_);
  show();
  qDebug() << "NNPCDialog::Show()";
}

void NPCDialog::TypingDone() {
  qDebug() << "NPCDialog::TypingDone";
  QTimer::singleShot(current_pause_, this, [this](){LoadNext();});
}

void NPCDialog::LoadNext() {
  qDebug() << "NPCDialog::LoadNext()";
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

void NPCDialog::CalculateSize(int current_size) {
  current_size_ = current_size;
  int cnt_wdth = container_->width();
  int cnt_height = container_->height();
  int hint_height = 25;
  int hint_width = current_size * 13;

  if (hint_width % cnt_wdth == 0) {
    hint_height = 25 * (hint_width / cnt_wdth);
    hint_width = cnt_wdth;
  } else {
    hint_height = 25 * (hint_width / cnt_wdth + 1);
    if (hint_width > cnt_wdth) {
      hint_width = cnt_wdth;
    }
  }

  setGeometry(cnt_wdth-hint_width,
              cnt_height-hint_height,
              hint_width, hint_height);
  qDebug() << "setGeometry";
  qDebug() << container_->width()-hint_width <<
      container_->height()-hint_height <<
      hint_width << hint_height;
}

void NPCDialog::resizeEvent(QResizeEvent* event) {
  CalculateSize(current_size_);
  update();
}

}  // namespace ui