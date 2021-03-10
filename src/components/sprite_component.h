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
  explicit SpriteComponent(const QPixmap& pixmap, SpriteLayer = SpriteLayer::kBackground);
  SpriteComponent(const QPixmap& pixmap, const QRect& area, const QRect& bounds,
                  SpriteLayer = SpriteLayer::kBackground);
  SpriteComponent(const QRect& target_area, SpriteLayer = SpriteLayer::kBackground);

  void SetGraphics(const QPixmap& pixmap, const QRect& area);

  QPixmap pixmap;
  QRect source_area;
  QRect target_area;
  SpriteLayer layer;
};

}  // namespace game

#endif  // SRC_COMPONENTS_SPRITE_COMPONENT_H_
