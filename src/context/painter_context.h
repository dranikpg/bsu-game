#ifndef SRC_CONTEXT_PAINTER_CONTEXT_H_
#define SRC_CONTEXT_PAINTER_CONTEXT_H_

#include <QPainter>

namespace context {

class PainterContext {
 public:
  void SetPainter(QPainter* painter);
  QPainter& GetPainter();

 private:
  QPainter* painter_;
};

}  // namespace context

#endif  // SRC_CONTEXT_PAINTER_CONTEXT_H_
