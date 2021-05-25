#include "pixmap_rect.h"

namespace utils {

QPixmap utils::PixmapRect::Copy() const {
  QPixmap tmp = pixmap.copy(rect);
  return tmp.scaled(tmp.width(), tmp.height());
}

PixmapRect::PixmapRect(const QPixmap& pixmap)
  : pixmap(pixmap), rect(pixmap.rect()) {}

PixmapRect::PixmapRect(const QPixmap& pixmap, const QRect& rect)
    : pixmap(pixmap), rect(rect) {}

}  // namespace utils
