#ifndef SRC_MAP_MAP_LOADER_H_
#define SRC_MAP_MAP_LOADER_H_

#include <string>
#include <utility>
#include <vector>
#include <optional>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

#include "map_instancer.h"
#include "map_object.h"

namespace map {

class MapLoader {
 public:
  class InvalidPathException : std::runtime_error {
   public:
    explicit InvalidPathException(const std::string& arg);
  };
  class JsonError : std::runtime_error {
   public:
    explicit JsonError(const std::string& arg);
  };
  class MissingLayerError : std::runtime_error {
   public:
    explicit MissingLayerError(const std::string& arg);
  };

  void Load(QFile file, MapInstancer* instancer);

 private:
    std::optional<QJsonObject> FindLayer(const QJsonArray& array, const QString& name);
    MapObject ParseMapObject(const QJsonObject& obj);

 private:
  const std::vector<std::pair<MapLayer, QString>> kLayers = {
          {MapLayer::kCollision, "collision"},
          {MapLayer::kEntity, "entity"}
      };
};

}  // namespace map

#endif  // SRC_MAP_MAP_LOADER_H_
