#include "animation.h"

#include <utility>

namespace resource {

Animation::Animation(const QPixmap& pixmap,
                     std::vector<QRect> frames,
                     std::vector<uint16_t> frame_lengths)
    : pixmap_(pixmap), frames_(std::move(frames)), frame_lengths_(std::move(frame_lengths)) {}

const QPixmap& Animation::GetPixmap() const {
  return pixmap_;
}

const QRect& Animation::GetFrame(int frame) const {
  return frames_[frame];
}

uint16_t Animation::GetFrameLength(int frame) const {
  return frame_lengths_[frame];
}

std::size_t Animation::GetFrameCount() const {
  return frames_.size();
}

}  // namespace resource
