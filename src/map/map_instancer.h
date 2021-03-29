#ifndef SRC_MAP_MAP_INSTANCER_H_
#define SRC_MAP_MAP_INSTANCER_H_

#include "map_layer.h"
#include "map_object.h"

namespace map {

class MapInstancer {
 public:
  virtual void CreateMap(QString path) = 0;
  virtual void CreateObject(MapLayer layer, const MapObject& object) = 0;
};

}  // namespace map

#endif  // SRC_MAP_MAP_INSTANCER_H_
