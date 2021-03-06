#ifndef SRC_WIDGETS_NPC_DIALOG_H_
#define SRC_WIDGETS_NPC_DIALOG_H_

#include <memory>
#include <vector>
#include <utility>

#include "../resources/dialog.h"
#include "typing_label.h"

#include <QWidget>
#include <QHBoxLayout>
#include <QTimer>

namespace ui {

class NPCDialog : public QWidget {
  Q_OBJECT

 public:
  enum class DialogType {
    kLeftBottom,
    kRightBottom
  };

  void resizeEvent(QResizeEvent *event) override;
  void paintEvent(QPaintEvent *event) override;
  using Callback = std::function<void()>;
  NPCDialog(std::vector<std::pair<QString, int>> parts,
            Callback callback, QWidget* container, DialogType type);

  void Start();
  void SetTypingStartCallback(Callback);
  void SetTypingEndCallback(Callback);

 private:
  DialogType type_;

  void TypingEnd();
  Callback typing_end_callback_;

  void TypingStart();
  Callback typing_start_callback_;

  void TypingDone();
  void LoadNext();

  Callback callback_;
  std::vector<std::pair<QString, int>> parts_;
  int current_part_;
  int current_pause_;
  QString current_text_;

  QWidget* container_;
  TypingLabel* label_;
  QHBoxLayout* main_layout_;

 private slots:
  void CalculateSize();
};

}  // namespace ui

#endif  // SRC_WIDGETS_NPC_DIALOG_H_
