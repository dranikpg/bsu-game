#include "dialog_parser.h"

#include <vector>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "../exceptions/invalid_path_exception.h"
#include "../exceptions/json_format_exception.h"

/*
 * text: string[]
 * options: map[]
 *
 */

namespace utils {

resource::Dialog DialogParser::Parse(QFile file) {
  using resource::Dialog;

  if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
    throw InvalidPathException(file);
  }
  QString json_src = file.readAll();
  QJsonDocument json_document = QJsonDocument::fromJson(json_src.toUtf8());
  QJsonObject root_object = json_document.object();

  std::vector<QString> text;
  for (const QJsonValueRef& obj: root_object.value("text").toArray()) {
    if (obj.isString()) {
      text.push_back(obj.toString());
    }
  }

  Dialog::OptionBag option_bag;
  QJsonObject option_object = root_object.value("options").toObject();
  for (const QString& key: option_object.keys()) {
    QJsonValueRef obj = option_object[key];
    if (obj.isString()) {
      option_bag[key] = obj.toString();
    }
  }

  return Dialog(std::move(text), std::move(option_bag));
}

}  // namespace utils
