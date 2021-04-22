#ifndef SRC_CONTEXT_MINI_GAME_CONTEXT_H_
#define SRC_CONTEXT_MINI_GAME_CONTEXT_H_

#include <QWidget>

namespace context {

class MiniGameContext {
 public:
  void Init(QWidget* container);
  void Start();
  void Stop();
  QWidget* GetContainer() const;
 private:
  bool active_;
  QWidget* container_;
};

}  // namespace context

#endif  // SRC_CONTEXT_MINI_GAME_CONTEXT_H_
