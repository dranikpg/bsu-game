#ifndef SRC_COMPONENTS_SPRITE_COMPONENT_H_
#define SRC_COMPONENTS_SPRITE_COMPONENT_H_

#include <QPixmap>
#include <QPointF>
#include <QRectF>

#include "../ecs/component.h"
#include "../constants/sprite_layer.h"

using constants::SpriteLayer;

namespace game {

struct SpriteComponent : public ecs::Component {
  explicit SpriteComponent(const QPixmap& pixmap, SpriteLayer = SpriteLayer::kBackground);
  SpriteComponent(const QPixmap& pixmap, const QRectF& area, const QRectF& bounds,
                  SpriteLayer = SpriteLayer::kBackground);
  explicit SpriteComponent(const QRectF& target_area, SpriteLayer = SpriteLayer::kBackground);

  void SetGraphics(const QPixmap& pixmap, const QRectF& area);

  QPixmap pixmap;
  QRectF source_area;
  QRectF target_area;
  SpriteLayer layer;
};

}  // namespace game

#endif  // SRC_COMPONENTS_SPRITE_COMPONENT_H_
