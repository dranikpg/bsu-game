#ifndef NPC_DIALOG_H
#define NPC_DIALOG_H

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
    void resizeEvent(QResizeEvent *event) override;
    using Callback = std::function<void()>;
    NPCDialog(std::vector<std::pair<QString, int>> parts,
              Callback callback, QWidget* container);

    void Start();

   private:
    void TypingDone();
    void LoadNext();

    Callback callback_;
    std::vector<std::pair<QString, int>> parts_;
    int current_part_;
    int current_pause_;
    int current_size_;
    QString current_text_;

    QWidget* container_;
    TypingLabel* label_;
    QHBoxLayout* main_layout_;

   private slots:
    void CalculateSize();
  };
}  // namespace ui

#endif //NPC_DIALOG_H
