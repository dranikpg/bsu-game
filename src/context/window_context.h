#ifndef SRC_CONTEXT_WINDOW_CONTEXT_H_
#define SRC_CONTEXT_WINDOW_CONTEXT_H_

#include <QSize>
#include <QTransform>

namespace context {

class WindowContext {
 public:
  const QSize& GetSize() const;
  void SetSize(const QSize& size);
  const QTransform& GetRenderTransform() const;
  void SetRenderTransform(const QTransform& render_transform);
 private:
  QSize size_;
  QTransform render_transform_;
};

}  // namespace context

#endif  // SRC_CONTEXT_WINDOW_CONTEXT_H_
