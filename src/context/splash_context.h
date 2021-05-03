#ifndef SRC_CONTEXT_SPLASH_CONTEXT_H_
#define SRC_CONTEXT_SPLASH_CONTEXT_H_

#include <functional>

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

#include "../widgets/typing_label.h"
#include "../utils/pixmap_rect.h"

namespace context {

class SplashContext {
 public:
  void Init(QWidget* container);
  void Show(utils::PixmapRect icon, QString text, std::function<void()> callback);
 private:
  std::function<void()> callback_;

  QWidget* container_;

  QLabel* icon_;
  QPushButton* button_;
  ui::TypingLabel* text_;
  QGridLayout* main_layout_;
};

}  // namespace context

#endif  // SRC_CONTEXT_SPLASH_CONTEXT_H_
