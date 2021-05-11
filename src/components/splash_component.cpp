#include "splash_component.h"

#include <utility>

namespace game {

SplashComponent::SplashComponent(const utils::PixmapRect& icon,
                                 QString text,
                                 FinishCallback callback)
    : icon(icon), text(std::move(text)), callback(std::move(callback)) {}

}  // namespace game
