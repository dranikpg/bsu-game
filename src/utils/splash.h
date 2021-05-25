#ifndef SRC_UTILS_SPLASH_H_
#define SRC_UTILS_SPLASH_H_

#include <utility>
#include <string>

#include <QString>

#include "pixmap_rect.h"

namespace utils {

class Splash {
 public:
  static std::pair<PixmapRect, QString> Load(const std::string& name);
};

}  // namespace utils

#endif  // SRC_UTILS_SPLASH_H_
