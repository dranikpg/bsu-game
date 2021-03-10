#ifndef WINDOW_CONTEXT_H
#define WINDOW_CONTEXT_H

#include <QSize>

namespace context {

class WindowContext {
 public:
  const QSize& GetSize() const;
  void SetSize(const QSize& size);
 private:
  QSize size_;
};

}  // namespace context

#endif  // WINDOW_CONTEXT_H
