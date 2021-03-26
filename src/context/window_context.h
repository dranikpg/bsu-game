#ifndef SRC_CONTEXT_WINDOW_CONTEXT_H_
#define SRC_CONTEXT_WINDOW_CONTEXT_H_

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

#endif  // SRC_CONTEXT_WINDOW_CONTEXT_H_
