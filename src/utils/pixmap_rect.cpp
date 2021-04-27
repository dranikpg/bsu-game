#include "pixmap_rect.h"

namespace utils {

QPixmap utils::PixmapRect::Copy() const {
  return pixmap.copy(rect);
}

PixmapRect::PixmapRect(const QPixmap& pixmap)
  : pixmap(pixmap), rect(pixmap.rect()) {}

PixmapRect::PixmapRect(const QPixmap& pixmap, const QRect& rect)
    : pixmap(pixmap), rect(rect) {}

}  // namespace utils
