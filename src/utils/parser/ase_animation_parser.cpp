#include "ase_animation_parser.h"

#include <utility>
#include <vector>

#include <QDebug>
#include <QString>
#include <QRect>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace utils {

AseAnimationParser::AnimationBag AseAnimationParser::Parse(QFile file) {
  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw InvalidPathException("Invalid file path: " + file.fileName().toStdString());
  }
  QString json_src = file.readAll();
  QJsonDocument json_document = QJsonDocument::fromJson(json_src.toUtf8());
  QJsonObject root_obj = json_document.object();

  // check pixmap path
  if (!root_obj.value("pixmap").isString()) {
    throw JsonFormatException("No pixmap property");
  }

  // load pixmap
  QString pixmap_path = root_obj.value("pixmap").toString();
  QPixmap pixmap;
  pixmap.load(pixmap_path);
  if (pixmap.isNull()) {
    throw InvalidPixmapException("Failed to load pixmap: " + pixmap_path.toStdString());
  }

  // parse frames to vector
  auto frames = ParseFrames(root_obj.value("frames").toArray());

  // parse animations
  AnimationBag animations;
  for (QJsonValueRef tag_ref: root_obj.value("meta").toObject().value("frameTags").toArray()) {
    QJsonObject tag_obj = tag_ref.toObject();

    std::string name = tag_obj.value("name").toString().toStdString();
    int start = tag_obj.value("from").toInt();
    int end = tag_obj.value("to").toInt(0);

    std::vector<QRect> frame_areas;
    std::vector<uint16_t> frame_lengths;

    for (int index = start; index <= end; index++) {
      frame_areas.push_back(frames[index].first);
      frame_lengths.push_back(frames[index].second);
    }

    animations[name] =
        std::make_shared<Animation>(pixmap, std::move(frame_areas), std::move(frame_lengths));
  }

  file.close();
  return animations;
}

QRect AseAnimationParser::ParseRect(const QJsonObject& obj) {
  return QRect(
      obj.value("x").toInt(0),
      obj.value("y").toInt(0),
      obj.value("w").toInt(0),
      obj.value("h").toInt(0));
}

std::vector<std::pair<QRect, uint16_t>> AseAnimationParser::ParseFrames(const QJsonArray& array) {
  std::vector<std::pair<QRect, uint16_t>> frames;
  for (QJsonValue frame_ref: array) {
    QJsonObject frame_obj = frame_ref.toObject();
    QRect frame_rect = ParseRect(frame_obj.value("frame").toObject());
    uint16_t frame_length = frame_obj.value("duration").toInt(1);
    frames.emplace_back(frame_rect, frame_length);
  }
  return frames;
}

AseAnimationParser::InvalidPixmapException::InvalidPixmapException(const std::string& arg)
    : runtime_error(arg) {}

AseAnimationParser::InvalidPathException::InvalidPathException(const std::string& arg)
    : runtime_error(arg) {}

AseAnimationParser::JsonFormatException::JsonFormatException(const std::string& arg)
    : runtime_error(arg) {}

}  // namespace utils
