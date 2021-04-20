//
// Created by Vladislav Oleshko on 19.04.21.
//

#include "pix_rect.h"

namespace utils {

QPixmap utils::PixRect::Copy() const {
  return pixmap.copy(rect);
}

PixRect::PixRect(const QPixmap& pixmap)
  : pixmap(pixmap), rect(pixmap.rect()) {}

PixRect::PixRect(const QPixmap& pixmap, const QRect& rect)
    : pixmap(pixmap), rect(rect) {}

}  // namespace utils
