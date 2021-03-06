#ifndef SRC_UTILS_PARSER_ASE_ANIMATION_PARSER_H_
#define SRC_UTILS_PARSER_ASE_ANIMATION_PARSER_H_

#include <string>
#include <memory>
#include <unordered_map>
#include <stdexcept>

#include <QFile>

#include "../../resources/animation.h"

using resource::Animation;

namespace utils {

class AseAnimationParser {
 public:
  class InvalidPixmapException : public std::runtime_error {
   public:
    InvalidPixmapException(const std::string& arg);
  };
  class InvalidPathException : public std::runtime_error {
   public:
    InvalidPathException(const std::string& arg);
  };
  class JsonFormatException : public std::runtime_error {
   public:
    JsonFormatException(const std::string& arg);
  };

  using AnimationBag = std::unordered_map<std::string, std::shared_ptr<Animation>>;
  static AnimationBag Parse(QFile file) noexcept(false);
 private:
  static QRect ParseRect(const QJsonObject& obj);
  static std::vector<std::pair<QRect, uint16_t>> ParseFrames(const QJsonArray& array);
};

}

#endif  // SRC_UTILS_PARSER_ASE_ANIMATION_PARSER_H_
