#ifndef SRC_CONTEXT_PAINTER_CONTEXT_H_
#define SRC_CONTEXT_PAINTER_CONTEXT_H_

#include <QPainter>

namespace context {

class PainterContext {
 public:
  void Supply(QPainter* painter);
  QPainter& Acquire();
 private:
  QPainter* painter_;
};

}  // namespace context

#endif  // SRC_CONTEXT_PAINTER_CONTEXT_H_
