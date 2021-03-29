#ifndef SRC_RESOURCES_PATH_H_
#define SRC_RESOURCES_PATH_H_

#include <vector>

#include <QPoint>

namespace resources {

class Path {
 public:
  struct WayPoint {
    WayPoint(int x, int y, int pause);
    QPoint point;
    int pauseFrames = 0;
  };
  explicit Path(std::vector<WayPoint>&& points);
  int Size() const;
  const WayPoint& Point(int idx) const;
 private:
  std::vector<WayPoint> points_;
};

}  // namespace resources

#endif  // SRC_RESOURCES_PATH_H_
