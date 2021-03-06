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
  SpriteComponent(const QPixmap& pixmap, const QRect& area, const QRect& bounds, SpriteLayer layer);
  explicit SpriteComponent(const QRect& bounds, SpriteLayer = SpriteLayer::kBackground);

  void SetGraphics(const QPixmap& pixmap, const QRect& area);

  QPixmap pixmap_;
  QRect source_area_;
  QRect target_area_;
  SpriteLayer layer_;
};

}  // namespace game

#endif  // SRC_COMPONENTS_SPRITE_COMPONENT_H_
