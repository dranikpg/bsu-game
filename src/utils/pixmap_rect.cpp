#include "pixmap_rect.h"

namespace utils {

QPixmap utils::PixmapRect::Copy() const {
  QPixmap tmp = pixmap.copy(rect);
  return tmp.scaled(tmp.width() * scale.x(), tmp.height() * scale.y());
}

PixmapRect::PixmapRect(const QPixmap& pixmap)
  : pixmap(pixmap), rect(pixmap.rect()) {}

PixmapRect::PixmapRect(const QPixmap& pixmap, const QRect& rect, const QPoint& scale)
    : pixmap(pixmap), rect(rect), scale(scale)  {}

}  // namespace utils
