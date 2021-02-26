#include "sprite_component.h"

namespace game {

SpriteComponent::SpriteComponent(const QPixmap& pixmap, SpriteLayer layer)
    : pixmap(pixmap), area(pixmap.rect()), bounds(pixmap.rect()),  layer(layer) {
}
SpriteComponent::SpriteComponent(const QPixmap& pixmap,
                                 const QRect& area,
                                 const QRect& bounds,
                                 SpriteLayer layer)
    : pixmap(pixmap), area(area), bounds(bounds), layer(layer) {}

void SpriteComponent::UpdateGraphics(const QPixmap& pixmap, const QRect& area) {
  this->pixmap = pixmap;
  this->area = area;
}

}  // namespace game
