#ifndef CHOOSE_WIDGET_H
#define CHOOSE_WIDGET_H

#include "../npc_dialog.h"
#include "choose_var_widget.h"

#include <QWidget>
#include <QPushButton>

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
    void paintEvent(QPaintEvent *event) override;
   private:
    void ShowMask();
    void HideMask();
    void ShowQuestion();
    void HideQuestion();
    void ShowVars();
    void HideVars();
    void MakeConnections();
    void RecalculateSizes();
    void Hide();
    void End();

    QWidget* container_;
    TypingLabel* question_label_;
    QString question_text_;
    ChooseVarWidget* var_1_;
    QWidget* var_1_container_;
    ChooseVarWidget* var_2_;
    QWidget* var_2_container_;
    ChooseVarWidget* var_3_;
    QWidget* var_3_container_;
    ChooseVarWidget* var_4_;
    QWidget* var_4_container_;
    std::vector<ChooseVarWidget*> vars_;

   private slots:
    void Var1ButClicked(bool);
    void Var2ButClicked(bool);
    void Var3ButClicked(bool);
    void Var4ButClicked(bool);

   private:
    Callback callback_;
    int* chosen_var_ret_;
    int current_alpha_ = 0;
    QTimer* timer_;

    int destroyed_choose_widgets_ = 0;
  };

}  // namespace ui

#endif //CHOOSE_WIDGET_H
