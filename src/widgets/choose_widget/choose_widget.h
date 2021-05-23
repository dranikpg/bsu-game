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
    using Callback = std::function<void(int)>;

    explicit ChooseWidget(QWidget* container,
                          const QString&  question,
                          const QString& var1,
                          const QString& var2,
                          const QString& var3,
                          const QString& var4,
                          Callback callback);
    void Start(QPixmap start_frame);
    void resizeEvent(QResizeEvent *event) override;

   private:
    QWidget* container_;
    ChooseVarWidget* question_widget_;
    QWidget* question_widget_container_;
    std::vector<ChooseVarWidget*> vars_;
    std::vector<QWidget*> vars_containers_;

   private slots:
    void Var1ButClicked(bool);
    void Var2ButClicked(bool);
    void Var3ButClicked(bool);
    void Var4ButClicked(bool);

    void ShowMask();
    void ShowQuestion();
    void ShowVars();
    void MakeConnections();
    void RecalculateSizes();
    void Hide();
    void End();

   private:
    Callback callback_;
    int chosen_var_ = -1;
    QLabel* mask_;
    QPixmap start_frame_ = {};
  };

}  // namespace ui

#endif //CHOOSE_WIDGET_H
