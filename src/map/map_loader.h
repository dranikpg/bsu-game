#ifndef SRC_MAP_MAP_LOADER_H_
#define SRC_MAP_MAP_LOADER_H_

#include <string>
#include <utility>
#include <vector>
#include <optional>

#include <QFile>
#include <QJsonArray>
#include <QJsonObject>

#include "../resources/path.h"
#include "map_instantiator.h"
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

  static void Load(QFile file, MapInstantiator* instancer);

 private:
    static std::optional<QJsonObject> FindLayer(const QJsonArray& array,
                                                const QString& name);
    static MapObject ParseMapObject(const QJsonObject& obj);
    static std::pair<resource::Path, QString> ParsePath(const QJsonObject& obj);
    static std::pair<QPoint, QString> ParsePoint(const QJsonObject& obj);


 private:
  static const std::vector<std::pair<MapLayer, QString>> kLayers;
};

}  // namespace map

#endif  // SRC_MAP_MAP_LOADER_H_
