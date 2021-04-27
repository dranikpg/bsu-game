#include "sprite_component.h"

namespace game {

SpriteComponent::SpriteComponent(const QPixmap& pixmap, SpriteLayer layer)
    : pixmap(pixmap), source_area(pixmap.rect()), target_area(pixmap.rect()), layer(layer) {
}

SpriteComponent::SpriteComponent(const QPixmap& pixmap,
                                 const QRectF& area,
                                 const QRectF& bounds,
                                 SpriteLayer layer)
    : pixmap(pixmap), source_area(area), target_area(bounds), layer(layer) {}

SpriteComponent::SpriteComponent(const QRectF& target_area, SpriteLayer layer)
    : target_area(target_area), layer(layer) {}

void SpriteComponent::SetGraphics(const QPixmap& pixmap, const QRectF& area) {
  this->pixmap = pixmap;
  this->source_area = area;
}

}  // namespace game
