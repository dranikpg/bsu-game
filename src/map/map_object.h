#ifndef SRC_MAP_MAP_OBJECT_H_
#define SRC_MAP_MAP_OBJECT_H_

#include <QPoint>
#include <QSize>
#include <QString>

namespace map {

struct MapObject {
  QPoint position;
  QSize size;
  QString name;
};

}  // namespace map

#endif  // SRC_MAP_MAP_OBJECT_H_
