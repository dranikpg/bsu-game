#ifndef SRC_MAP_MAP_INSTANTIATOR_H_
#define SRC_MAP_MAP_INSTANTIATOR_H_

#include "map_layer.h"
#include "map_object.h"

#include "../resources/path.h"

namespace map {

class MapInstantiator {
 public:
  virtual void CreateMap(const QString& path) = 0;
  virtual void CreateObject(MapLayer layer, const MapObject& object) = 0;
  virtual void CreatePath(const resources::Path& path, const QString& name) = 0;
};

}  // namespace map

#endif  // SRC_MAP_MAP_INSTANTIATOR_H_
