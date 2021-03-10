#include "painter_context.h"

namespace context {

void PainterContext::SetPainter(QPainter* painter) {
  painter_ = painter;
}

QPainter& PainterContext::GetPainter() {
  return *painter_;
}

}  // namespace context
