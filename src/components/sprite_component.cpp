#include "sprite_component.h"

namespace game {

SpriteComponent::SpriteComponent(const QPixmap& pixmap, SpriteLayer layer)
    : pixmap(pixmap), source_area(pixmap.rect()), target_area(pixmap.rect()), layer(layer) {
}

SpriteComponent::SpriteComponent(const QPixmap& pixmap,
                                 const QRect& area,
                                 const QRect& bounds,
                                 SpriteLayer layer)
    : pixmap(pixmap), source_area(area), target_area(bounds), layer(layer) {}

void SpriteComponent::UpdateGraphics(const QPixmap& pixmap, const QRect& area) {
  this->pixmap = pixmap;
  this->source_area = area;
}

}  // namespace game
