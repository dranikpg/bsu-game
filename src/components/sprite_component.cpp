#include "sprite_component.h"

namespace game {

SpriteComponent::SpriteComponent(const QPixmap& pixmap, SpriteLayer layer)
    : pixmap_(pixmap), source_area_(pixmap.rect()), target_area_(pixmap.rect()), layer_(layer) {
}

SpriteComponent::SpriteComponent(const QPixmap& pixmap,
                                 const QRect& area,
                                 const QRect& bounds,
                                 SpriteLayer layer)
    : pixmap_(pixmap), source_area_(area), target_area_(bounds), layer_(layer) {
}

SpriteComponent::SpriteComponent(const QRect& bounds, SpriteLayer layer)
    : target_area_(bounds), layer_(layer) {
}

void SpriteComponent::SetGraphics(const QPixmap& pixmap, const QRect& area) {
  this->pixmap_ = pixmap;
  this->source_area_ = area;
}

}  // namespace game
