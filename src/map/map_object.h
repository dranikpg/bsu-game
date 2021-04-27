#ifndef SRC_MAP_MAP_OBJECT_H_
#define SRC_MAP_MAP_OBJECT_H_

#include <QPointF>
#include <QSizeF>
#include <QString>

namespace map {

struct MapObject {
  QPointF position;
  QSizeF size;
  QString name;
};

}  // namespace map

#endif  // SRC_MAP_MAP_OBJECT_H_
