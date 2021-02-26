#ifndef SRC_RESOURCES_ANIMATION_H_
#define SRC_RESOURCES_ANIMATION_H_

#include <vector>

#include <QPixmap>
#include <QRect>

namespace resource {

class Animation {
 public:
  Animation(const QPixmap& pixmap,
            std::vector<QRect> frames,
            std::vector<uint16_t> frame_lengths);
  const QPixmap& GetPixmap() const;
  const QRect& GetFrame(int frame) const;
  uint16_t GetFrameLength(int frame) const;
  std::size_t GetFrameCount() const;
 private:
  QPixmap pixmap_;
  std::vector<QRect> frames_;
  std::vector<uint16_t> frame_lengths_;
  size_t GetFrameCount();
};

}  // namespace resource

#endif  // SRC_RESOURCES_ANIMATION_H_
