#ifndef SRC_COMPONENTS_SPLASH_COMPONENT_H_
#define SRC_COMPONENTS_SPLASH_COMPONENT_H_

#include <functional>

#include "../ecs/component.h"
#include "../utils/pixmap_rect.h"

namespace game {

struct SplashComponent : public ecs::Component {
  using FinishCallback = std::function<void()>;

  SplashComponent(const utils::PixmapRect& icon,
                  QString text,
                  FinishCallback callback);

  utils::PixmapRect icon;
  QString text;
  FinishCallback callback;
};

}  // namespace game

#endif  // SRC_COMPONENTS_SPLASH_COMPONENT_H_
