#include "painter_context.h"

namespace context {

void PainterContext::Supply(QPainter* painter) {
  painter_ = painter;
}

QPainter& PainterContext::Acquire() {
  return *painter_;
}

}  // namespace context
