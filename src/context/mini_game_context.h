#ifndef SRC_CONTEXT_MINI_GAME_CONTEXT_H_
#define SRC_CONTEXT_MINI_GAME_CONTEXT_H_

#include <QWidget>
#include <QTransform>

namespace context {

class MiniGameContext {
 public:
  void Init(QWidget* container);
  void Start();
  void Stop();
  QWidget* GetContainer() const;
  const QTransform& GetRenderTransform() const;
  void SetRenderTransform(const QTransform& render_transform);

 private:
  bool active_;
  QTransform render_transform_;
  QWidget* container_;
};

}  // namespace context

#endif  // SRC_CONTEXT_MINI_GAME_CONTEXT_H_
