#ifndef SRC_UTILS_PIXMAP_RECT_H_
#define SRC_UTILS_PIXMAP_RECT_H_

#include <QPixmap>
#include <QRect>

namespace utils {

struct PixmapRect {
  PixmapRect() = default;
  explicit PixmapRect(const QPixmap& pixmap);
  PixmapRect(const QPixmap& pixmap, const QRect& rect);
  QPixmap Copy() const;

  QPixmap pixmap;
  QRect rect;
};

}  // namespace utils

#endif  // SRC_UTILS_PIXMAP_RECT_H_
