#ifndef SRC_UTILS_PIX_RECT_H_
#define SRC_UTILS_PIX_RECT_H_

#include <QPixmap>
#include <QRect>

namespace utils {

struct PixRect {
  PixRect() = default;
  explicit PixRect(const QPixmap& pixmap);
  PixRect(const QPixmap& pixmap, const QRect& rect);
  QPixmap Copy() const;

  QPixmap pixmap;
  QRect rect;
};

}  // namespace utils

#endif  // SRC_UTILS_PIX_RECT_H_
