#include "map_loader.h"

#include <algorithm>

#include <QDebug>

#include <QJsonValue>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

namespace map {

void MapLoader::Load(QFile file, MapInstancer* instancer) {
  // read file
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw InvalidPathException("Invalid file path: " + file.fileName().toStdString());
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
    throw MissingLayerError("No image layer");
  }

  // parse layers
  for (const auto& layer_id: kLayers) {
    std::optional<QJsonObject> layer = FindLayer(layer_array, layer_id.second);
    if (!layer) {
      throw MissingLayerError("No layer " + layer_id.second.toStdString());
    }
    for (const auto& obj_refs : (*layer).value("objects").toArray()) {
      MapObject obj = ParseMapObject(obj_refs.toObject());
      instancer->CreateObject(layer_id.first, obj);
    }
  }
}

std::optional<QJsonObject> MapLoader::FindLayer(const QJsonArray& array, const QString& name) {
  auto layer_it = std::find_if(array.begin(), array.end(), [&name](const QJsonValue& val) {
    return val.toObject().value("name") == name;
  });
  if (layer_it != array.end()) {
    return (*layer_it).toObject();
  } else {
    return std::nullopt;
  }
}

MapObject MapLoader::ParseMapObject(const QJsonObject& obj) {
  MapObject map_obj;
  map_obj.position.rx() = obj.value("x").toInt();
  map_obj.position.ry() = obj.value("y").toInt();
  map_obj.size.rwidth() = obj.value("width").toInt();
  map_obj.size.rheight() = obj.value("height").toInt();
  map_obj.name = obj.value("name").toString();
  return map_obj;
}

MapLoader::JsonError::JsonError(const std::string& arg) : runtime_error(arg) {}

MapLoader::MissingLayerError::MissingLayerError(const std::string& arg) : runtime_error(arg) {}

MapLoader::InvalidPathException::InvalidPathException(const std::string& arg)
    : runtime_error(arg) {}

}  // namespace map
