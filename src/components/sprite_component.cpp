#include "sprite_component.h"

namespace game {

SpriteComponent::SpriteComponent(const QPixmap& pixmap, SpriteLayer layer)
    : pixmap(pixmap), area(pixmap.rect()), size(pixmap.rect()), offset(0, 0), layer(layer) {
}

SpriteComponent::SpriteComponent(const QPixmap& pixmap, const QRect& area,
                                 const QRect& size, const QPoint& offset,
                                 SpriteLayer layer)
    : pixmap(pixmap), area(area), size(size), offset(offset), layer(layer) {}

}  // namespace game
