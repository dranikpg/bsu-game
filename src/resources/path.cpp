#include "path.h"
#include <utility>

namespace resource {

Path::WayPoint::WayPoint(int x, int y, int pause = 0)
  : point(x, y), pauseFrames(pause) {}

const Path::WayPoint& Path::Point(int idx) const {
  return points_[idx % Size()];
}

int Path::Size() const {
  return points_.size();
}

void Path::ReversePath() {
  std::reverse(points_.begin(), points_.end());
}

Path::Path(std::vector<WayPoint>&& points) : points_(std::move(points)) {}

}  // namespace resource
