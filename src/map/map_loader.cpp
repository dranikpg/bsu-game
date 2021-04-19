#include "map_loader.h"

#include <algorithm>
#include <cmath>

#include <QDebug>
#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace map {

void MapLoader::Load(QFile file, MapInstantiator* instancer) {
  // read file
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw InvalidPathException(file.fileName().toStdString());
  }
  QString json_src = file.readAll();
  QJsonDocument json_document = QJsonDocument::fromJson(json_src.toUtf8());
  QJsonObject root_obj = json_document.object();

  QJsonArray layer_array = root_obj.value("layers").toArray();
  // parse image layer
  std::optional<QJsonObject> image_layer = FindLayer(layer_array, "background");
  if (image_layer) {
    QString image_path = (*image_layer).value("image").toString();
    instancer->CreateMap(image_path);
  } else {
    throw MissingLayerError("image");
  }

  // parse paths
  auto path_layer = FindLayer(layer_array, "path");
  if (path_layer) {
    for (const auto& obj_ref : (*path_layer).value("objects").toArray()) {
      auto named_path = ParsePath(obj_ref.toObject());
      instancer->CreatePath(std::move(named_path.first), named_path.second);
    }
  }

  // parse layers
  for (const auto& layer_id: kLayers) {
    std::optional<QJsonObject> layer = FindLayer(layer_array, layer_id.second);
    for (const auto& obj_ref : (*layer).value("objects").toArray()) {
      MapObject obj = ParseMapObject(obj_ref.toObject());
      instancer->CreateObject(layer_id.first, obj);
    }
  }

  file.close();
}

std::optional<QJsonObject> MapLoader::FindLayer(const QJsonArray& array,
                                                const QString& name) {
  auto layer_it =
      std::find_if(array.begin(), array.end(), [&name](const QJsonValue& val) {
        return val.toObject().value("name") == name;
      });
  if (layer_it != array.end()) {
    return layer_it->toObject();
  } else {
    return std::nullopt;
  }
}

MapObject MapLoader::ParseMapObject(const QJsonObject& obj) {
  MapObject map_obj;
  auto [position, name] = ParsePoint(obj);
  map_obj.name = std::move(name);
  map_obj.size.rwidth() = static_cast<int>(obj.value("width").toDouble());
  map_obj.size.rheight() = static_cast<int>(obj.value("height").toDouble());
  map_obj.position = position + QPoint(map_obj.size.rwidth() / 2,
                                       map_obj.size.rheight() / 2);
  return map_obj;
}

std::pair<QPoint, QString> MapLoader::ParsePoint(const QJsonObject& obj) {
  float x = obj.value("x").toDouble();
  float y = obj.value("y").toDouble();
  QString name = obj.value("name").toString();
  return {QPoint(x, y), std::move(name)};
}


std::pair<resource::Path, QString> MapLoader::ParsePath(const QJsonObject& obj) {
  auto[pos, name] = ParsePoint(obj);
  using resource::Path;
  std::vector<Path::WayPoint> points;
  for (const auto& ref: obj.value("polygon").toArray()) {
    const auto point_obj = ref.toObject();
    int x = static_cast<int>(point_obj.value("x").toDouble());
    int y = static_cast<int>(point_obj.value("y").toDouble());
    points.emplace_back(x + pos.x(), y + pos.y(), 0);
  }
  return {Path(std::move(points)), std::move(name)};
}

MapLoader::JsonError::JsonError(const std::string& arg) : runtime_error(arg) {}

MapLoader::MissingLayerError::MissingLayerError(const std::string& arg)
    : runtime_error("No layer " + arg) {}

MapLoader::InvalidPathException::InvalidPathException(const std::string& arg)
    : runtime_error("Invalid file path: " + arg) {}

const std::vector<std::pair<MapLayer, QString>> MapLoader::kLayers = {
    {MapLayer::kCollision, "collision"},
    {MapLayer::kEntity, "entity"}
};

}  // namespace map
