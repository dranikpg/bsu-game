#include "path.h"
#include <utility>

namespace resources {

Path::WayPoint::WayPoint(int x, int y, int pause = 0)
  : point(x, y), pauseFrames(pause) {
}

const Path::WayPoint& Path::Point(int idx) const {
  return points[idx % Size()];
}

int Path::Size() const {
  return points.size();
}

Path::Path(std::vector<WayPoint>&& points) : points(std::move(points)) {}

}  // namespace resources
