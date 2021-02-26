#ifndef SRC_COMPONENTS_SPRITE_COMPONENT_H_
#define SRC_COMPONENTS_SPRITE_COMPONENT_H_

#include <QPixmap>
#include <QPoint>
#include <QRect>

#include "../ecs/component.h"
#include "../constants/sprite_layer.h"

using constants::SpriteLayer;

namespace game {

struct SpriteComponent : public ecs::Component {
  explicit SpriteComponent(const QPixmap& pixmap, SpriteLayer = SpriteLayer::BACKGROUND);
  SpriteComponent(const QPixmap& pixmap,
                  const QRect& area,
                  const QRect& size,
                  const QPoint& offset,
                  SpriteLayer layer);
  QPixmap pixmap;
  QRect area;
  QRect size;
  QPoint offset;
  SpriteLayer layer;
};

}  // namespace game

#endif  // SRC_COMPONENTS_SPRITE_COMPONENT_H_
