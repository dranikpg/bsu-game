#ifndef SRC_CONTEXT_DIALOG_CONTEXT_H_
#define SRC_CONTEXT_DIALOG_CONTEXT_H_

#include <functional>
#include <map>

#include <QString>
#include <QPixmap>
#include <QWidget>
#include <QLabel>

#include "../utils/pix_rect.h"

namespace context {

class DialogContext {
 public:
  using AnswerHandler = std::function<void(QString)>;
  using OptionBag = std::map<QString, QString>;

  void Init(QWidget* container);
  void Show(const utils::PixRect& pixrect);
  void SetText(const QString& message);
  void SetQuestions(const OptionBag& options, AnswerHandler handler);
  void Hide();
 private:
  QWidget* container_;
  QLabel* text_label_;
  QLabel* image_label_;
};

}  // namespace context

#endif  // SRC_CONTEXT_DIALOG_CONTEXT_H_
