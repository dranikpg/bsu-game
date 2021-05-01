#ifndef SRC_RESOURCES_PATH_H_
#define SRC_RESOURCES_PATH_H_

#include <vector>

#include <QPoint>

namespace resource {

class Path {
 public:
  struct WayPoint {
    WayPoint(float x, float y, int pause);
    QPointF point;
    int pauseFrames = 0;
  };
  Path() = default;
  explicit Path(std::vector<WayPoint>&& points);
  explicit Path(QPointF target);
  int Size() const;
  const WayPoint& Point(int idx) const;
  bool Empty() const;
  void ReversePath();

 private:
  std::vector<WayPoint> points_;
};

}  // namespace resource

#endif  // SRC_RESOURCES_PATH_H_
