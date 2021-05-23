#ifndef CHOOSE_VAR_WIDGET_H
#define CHOOSE_VAR_WIDGET_H

#include <unordered_map>
#include <memory>

#include "../clickable_label.h"
#include "../../utils/parser/ase_animation_parser.h"

#include <QWidget>
#include <QString>
#include <QTimer>
#include <QPainter>
#include <QLabel>

namespace ui {

class ChooseVarWidget : public QWidget {
  Q_OBJECT

 public:
  enum class ChooseVarWidgetType {
    kQuestion,
    kAnswer
  };

  explicit ChooseVarWidget(QWidget* container,
                           const QString& text,
                           ChooseVarWidgetType type = ChooseVarWidgetType::kAnswer);
  void resizeEvent(QResizeEvent* event) override;
  void paintEvent(QPaintEvent *event) override;

  signals:
  void clicked(bool);
  void finishedAnimation(bool);

 private slots:
  void PlayAnimation(bool);

 private:
  void RecalculateSizes();

  QWidget* container_;
  ClickableLable* label_;

  ChooseVarWidgetType type_;
  QPixmap background_;
  QPixmap current_;

  std::shared_ptr<resource::Animation> animations_;

  QTimer* timer_;
};

}  // namespace ui

#endif //CHOOSE_VAR_WIDGET_H
