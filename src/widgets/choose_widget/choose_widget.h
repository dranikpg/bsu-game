#ifndef CHOOSE_WIDGET_H
#define CHOOSE_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include "../npc_dialog.h"

namespace ui {
  class ChooseWidget : public QWidget {
    Q_OBJECT
   public:
    using Callback = std::function<void()>;

    explicit ChooseWidget(QWidget* container);
    void Start(const QString& question,
               const QString& var1,
               const QString& var2,
               const QString& var3,
               const QString& var4,
               Callback callback,
               int* chosen_var_ret);
    void resizeEvent(QResizeEvent *event) override;
   private:
    void ShowButtons();
    void RecalculateSizes();
    void End();

    QWidget* container_;
    TypingLabel* question_label_;
    QPushButton* var_1_but_;
    QPushButton* var_2_but_;
    QPushButton* var_3_but_;
    QPushButton* var_4_but_;

   private slots:
    void Var1ButClicked(bool);
    void Var2ButClicked(bool);
    void Var3ButClicked(bool);
    void Var4ButClicked(bool);

   private:
    Callback callback_;
    int* chosen_var_ret_;
  };

}  // namespace ui

#endif //CHOOSE_WIDGET_H
