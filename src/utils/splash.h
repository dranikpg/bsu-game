#ifndef BSU_GAME_SRC_UTILS_SPLASH_H_
#define BSU_GAME_SRC_UTILS_SPLASH_H_

#include <utility>

#include <QString>

#include "pixmap_rect.h"

namespace utils {

class Splash {
 public:
  static std::pair<PixmapRect, QString> Load(const std::string& name);
};

}  // namespace utils

#endif  // BSU_GAME_SRC_UTILS_SPLASH_H_
